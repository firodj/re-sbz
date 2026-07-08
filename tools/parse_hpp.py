import json

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
from cxxheaderparser.types import Type, Pointer, Array, FunctionType, AnonymousName,  FundamentalSpecifier, NameSpecifier

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


class HxxParser:
    def __init__(self):
        self.data = None
        self.classes = dict()
        self.typedefs = dict()
        self.fundamental = {
            'void': 0,
            '_BYTE': 1,

            'bool': 1,
            'int': 4,
            'unsigned int': 4 ,
            'char': 1,
            'float': 4,
            'short': 2,
        }

        self.ptr_size = 4 # 32-bit

        self.depth = 0
        self.unions = 0

    def _populate_classes(self, classes):
        for c in classes:
            k = "::".join([k.format() for k in c.class_decl.typename.segments])
            self.classes[k] = c
            self._populate_classes(c.classes)

            # print("- name:", c.class_decl.typename.format())
            # print("  fields:")
            # for f in c.fields:
            #     print("  - name:", f.name)
            #     print("    type:", f.type.format())

    def parse(self, fname):
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
        self.data = data

        for f in data.namespace.functions:
            print("- name:", f.name.format()),
            print("  return_type:", f.return_type.format())
            print("  msvc_convention:", f.msvc_convention)
            print("  parameters:")
            for p in f.parameters:
                print("  - type:", p.type.format())
                if p.name != None:
                    print("    name:", p.name)


        self._populate_classes(data.namespace.classes)

        for t in data.namespace.typedefs:
            k = t.name
            self.typedefs[k] = t
        
    def calc_type(self, name):
        try:
            self.depth += 1

            if name in self.typedefs:
                return self._calc_typedef(name)
                
            elif name in self.classes:
                return self._calc_class(name)

            elif name in self.fundamental:
                return dict(kind='fundamental', name=name, size=self.fundamental[name])

            else:
                raise NotImplementedError(f"Unknown how to calc {name}")
        finally:
            self.depth -= 1
    
    def _skim_type(self, name):
        if name in self.typedefs:
            return dict(kind='typedef', name=name)
            
        elif name in self.classes:
            return dict(kind='class', name=name)

        elif name in self.fundamental:
            return dict(kind='fundamental', name=name)

        else:
            raise NotImplementedError(f"Unknown not found to skim name {name}")

    def _skim_field(self, field_type):
        if isinstance(field_type, Pointer):
            ptr_to = self._skim_field(field_type.ptr_to)
            return dict(kind='pointer', ptr_to=ptr_to)
        elif isinstance(field_type, Type):
            if len(field_type.typename.segments) != 1:
                raise NotImplementedError("expected segments should be 1")
            #classkey = field_type.typename.classkey
            typename = field_type.typename.segments[0]

            # if isinstance(typename, NameSpecifier):
            #     pass
            # elif isinstance(typename, FundamentalSpecifier):
            #     pass
            # else:
            #     print(field_type)

            return self._skim_type(typename.format())
        elif isinstance(field_type, FunctionType):
            return_type = self._skim_field(field_type.return_type)
            parameters = [self._skim_field(param.type) for param in field_type.parameters]
            return dict(kind='function', msvc_convention=field_type.msvc_convention, return_type=return_type, parameters=parameters)
        else:
            raise NotImplementedError(f"unknown how to skim type {type(field_type)}")

    def _calc_field(self, field_type):
        if isinstance(field_type, Pointer):
            ptr_to = self._skim_field(field_type.ptr_to)
            return dict(size=self.ptr_size, kind='pointer', ptr_to=ptr_to)

        elif isinstance(field_type, Type):
            if len(field_type.typename.segments) != 1:
                raise NotImplementedError("expected segments should be 1")
            #classkey = field_type.typename.classkey
            typename = field_type.typename.segments[0]

            # if isinstance(typename, NameSpecifier):
            #     pass
            # elif isinstance(typename, FundamentalSpecifier):
            #     pass
            # else:
            #     print(field_type)

            return self.calc_type(typename.format())
   
        elif isinstance(field_type, Array):
            array_of = self._calc_field(field_type.array_of)
            array_size = array_of['size']

            count = int(field_type.size.format(), 0)

            return dict(kind='array', size=count * array_size, count=count, array_of=array_of)

        else:
            raise NotImplementedError(f"unknown how to calc type {type(field_type)}")

    def _calc_typedef(self, name):
        type = self._calc_field(self.typedefs[name].type)
        return dict(kind='typedef', name=name, type=type, size=type['size'])

    def _calc_class(self, name):
        fields = self.classes[name].fields
        classkey = self.classes[name].class_decl.typename.classkey

        sizes = []
        is_union = classkey == 'union'
        offs = 0
        field_infos = []
        for i,field in enumerate(fields):

            #print("  "*self.depth, hex(offs), i, field.name, type(field.type))
           
            type_info = self._calc_field(field.type)
            field_size = type_info['size']
            
            field_info=dict(offs=offs, name=field.name, size=field_size, type=type_info)
            field_infos.append(field_info)

            sizes.append(field_size)

            if not is_union:
                offs += field_size
        
        if len(sizes) == 0:
            return dict(kind='class', name=name, classkey=classkey, fields=field_infos, size=0)

        if is_union:
            # Todo check
            return dict(kind='class', name=name, classkey=classkey, fields=field_infos, size=sizes[0])
        
        return dict(kind='class', name=name, classkey=classkey, fields=field_infos, size=sum(sizes))

if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print("missing argument")
        sys.exit(-1)

    hxxparser = HxxParser()
    hxxparser.parse(sys.argv[1])

    namez = [
        'D3DMATRIX',
        'SBSpecialScene',
        'CSBZGlobal',
    ][2]

    info = hxxparser.calc_type(namez)
    print(namez, json.dumps(info, indent=4))

    # size = hxxparser.calc_type('Cebol')
    # print('Cebol', size)

    
            