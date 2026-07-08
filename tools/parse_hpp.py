msvc_keywords = {'__int64'}

from cxxheaderparser.lexer import PlyLexer
PlyLexer.keywords |= msvc_keywords
#print(PlyLexer.keywords)

from cxxheaderparser.parser import CxxParser
CxxParser._compound_fundamentals |= msvc_keywords
CxxParser._fundamentals |= msvc_keywords
CxxParser._pqname_start_tokens |= msvc_keywords
#print(CxxParser._fundamentals)

from cxxheaderparser.simple import parse_file
from cxxheaderparser.options import ParserOptions
from cxxheaderparser.preprocessor import PreprocessorError, make_pcpp_preprocessor

import os, sys, io
import re
import typing
from pcpp import Preprocessor, OutputDirective, Action

def striptoks(toks):
    newtoks = []
    for tok in toks:
        if tok.type == 'CPP_WS': continue
        if tok.type == 'CPP_LINECONT': continue
        newtoks.append(tok)
   
    if newtoks[0].type == 'CPP_LPAREN' and newtoks[-1].type == 'CPP_RPAREN':
        newtoks = newtoks[1:-1]
    
    return newtoks

def toksarguments(toks):
    if len(toks) < 2: return
    
   
    cnt = 0
    found = False
    for i in range(len(toks)):
        if i == 0:
            if toks[0].type != 'CPP_LPAREN':
                return
            else:
                #print(toks)
                continue
      
        cnt = i
        if toks[i].type == 'CPP_RPAREN':
            found = True
            break

    if found:
        #print("FOUND", cnt+1)
        return toks[0:cnt+1]
    
class MyCustomPreprocessor(Preprocessor):
    def __init__(self,
        filename: str,
        encoding: typing.Optional[str],
        passthru_includes: typing.Optional["re.Pattern"],
        ):
        Preprocessor.__init__(self)
        self.filename = filename
        self.t_WS = self.t_WS + self.t_COMMENT
        
        self.errors: typing.List[str] = []
        self.assume_encoding = encoding
        self.passthru_includes = passthru_includes

    def on_error(self, file, line, msg):
        self.errors.append(f"{file}:{line} error: {msg}")

    def on_include_not_found(self, *ignored):
        raise OutputDirective(Action.IgnoreAndPassThrough)

    def on_comment(self, *ignored):
        return True
    
    def on_directive_handle(self,directive,toks,ifpassthru,precedingtoks):
    
        if directive.value == 'define' and toks[0].source == self.filename:

            name = toks[0].value
            arglist = None
            defined = None
            #print("%s:%d %s" % (toks[0].source, toks[0].lineno, name))
            if len(toks) > 1:
                toksargs = toksarguments(toks[1:])
                if toksargs != None:
                    arglist = "".join([tok.value for tok in toksargs])
                    toks = toks[len(toksargs):]
                if len(toks) > 1 and toks[1].type == 'CPP_WS':
                    args = striptoks(toks[2:])
                    defined = "".join([tok.value for tok in args])
                
                if arglist:
                    print("%s:%d %s%s = %s" % (toks[0].source, toks[0].lineno, name, arglist, defined))
                else:
                    print("%s:%d %s = %s" % (toks[0].source, toks[0].lineno, name, defined))
                
        return super().on_directive_handle(directive,toks,ifpassthru,precedingtoks)

def my_pcpp_filter(
    fname: str, fp: typing.TextIO, deps: typing.Optional[typing.Dict[str, bool]]
) -> str:
    # the output of pcpp includes the contents of all the included files, which
    # isn't what a typical user of cxxheaderparser would want, so we strip out
    # the line directives and any content that isn't in our original file

    line_ending = f'{fname}"\n'

    new_output = io.StringIO()
    keep = True

    for line in fp:
        if line.startswith("#line"):
            keep = line.endswith(line_ending)
            if deps is not None:
                start = line.find('"')
                deps[line[start + 1 : -2]] = True

        if keep:
            new_output.write(line)

    new_output.seek(0)
    return new_output.read()

def fixidapro(content):
    versions = dict()
    #result = re.sub(r'(\w+)(::$[.+])$', '\1*', content)
    def namer(m):
        n = m.group(1)
        if n in versions:
            versions[n] += 1
        else:
            versions[n] = 1
        i = versions[n]
        return f"{n}_{i}"

    content = re.sub(r'(\w+)(::\$[:$0-9A-Z]+)', lambda m: namer(m), content, flags=re.MULTILINE)

    content = re.sub(r' (__hex|__dec|__udec|__bin|__strlit\(.+?\))([ ;])', '\\2', content, flags=re.MULTILINE)

    content = re.sub(r'(__noreturn|__unaligned|__hidden|__fixed) ', '', content, flags=re.MULTILINE)
    
    content = re.sub(r'\*this', '*This', content, flags=re.MULTILINE)
    content = re.sub(r'int this', 'int *This', content, flags=re.MULTILINE)

    content = re.sub(r'^(typedef .+ wchar_t);', '// \\1', content, flags=re.MULTILINE)

    return content

def my_make_pcpp_preprocessor(
        defines: typing.List[str] = [],
        include_paths: typing.List[str] = [],
        retain_all_content: bool = False,
        encoding: typing.Optional[str] = None,
        passthru_includes: typing.Optional["re.Pattern"] = None,
    ):

    def _preprocess_file(filename: str, content: typing.Optional[str]) -> str:
        pp = MyCustomPreprocessor(filename, encoding, passthru_includes)
        if include_paths:
            for p in include_paths:
                pp.add_path(p)

        for define in defines:
            pp.define(define)

        if not retain_all_content:
            pp.line_directive = "#line"
        
        if content is None:
            with open(filename, "r", encoding=encoding) as cfp:
                content = cfp.read()

        content = fixidapro(content)

        pp.parse(content, filename)

        if pp.errors:
            raise PreprocessorError("\n".join(pp.errors))
        elif pp.return_code:
            raise PreprocessorError("failed with exit code %d" % pp.return_code)

        fp = io.StringIO()
        pp.write(fp)
        fp.seek(0)

        result = fp.read()
        with open("temp.i", "w") as ppoutput:
            ppoutput.write(result)

        fp.seek(0)

        if retain_all_content:
            result = fp.read()
        else:
            deps: typing.Optional[typing.Dict[str, bool]] = None
            abssource = os.path.abspath(filename)

            for rewrite in pp.rewrite_paths:
                temp = re.sub(rewrite[0], rewrite[1], abssource)
                if temp != abssource:
                    filename = temp
                    if os.sep != "/":
                        filename = filename.replace(os.sep, "/")
                    break

            deps = {}
            result = my_pcpp_filter(filename, fp, deps)

            with open("temp-filtered.i", "w") as ppoutput:
                ppoutput.write(result)

        return result
    
    return _preprocess_file

data = None
classes = dict()

def parse(fname):
    global data, classes
    
    opt = ParserOptions(
        verbose=False,
        preprocessor=my_make_pcpp_preprocessor(
            defines=[
                "_MSC_EXTENSIONS",
                "_INTEGRAL_MAX_BITS 64",
                "_MSC_VER 1400",
                "_MSC_FULL_VER 140050727",
                "_WIN32",
                "_M_IX86 600",
                "_M_IX86_FP 0",
                "_MT",
                "_UNICODE",
                "UNICODE",
                "_cdecl __cdecl",
                #'__w64',
                #'__int64 int64_t',
                #"_WCHAR_T_DEFINED",
            ],
            include_paths=[
                "./VC/PlatformSDK/Include",
                "./VC/include",
            ],
            retain_all_content = False,
        ),
    )
    data = parse_file(fname, options=opt)
    for f in data.namespace.functions:
        print("- name:", f.name.format()),
        print("  return_type:", f.return_type.format())
        print("  msvc_convention:", f.msvc_convention)
        print("  parameters:")
        for p in f.parameters:
            print("  - type:", p.type.format())
            if p.name != None:
                print("    name:", p.name)

    for c in data.namespace.classes:
        k = "::".join([k.name for k in c.class_decl.typename.segments])
        classes[k] = c
        print("- name:", c.class_decl.typename.format())
        print("  fields:")
        for f in c.fields:
            print("  - name:", f.name)
            print("    type:", f.type.format())

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print("missing argument")
        sys.exit(-1)
    parse(sys.argv[1])
