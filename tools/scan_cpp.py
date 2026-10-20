#!/usr/bin/env python3
"""
scan_cpp.py - inventory a reverse-engineered C++ source file (hnhmods style).

For each input file, reports:
  * structs defined
  * global variables: data refs / array refs / function-pointer imports,
    with the trailing `// once!` / `// WRITABLE` note when present
  * implemented functions, and for each:
      - DebugLog("[HNH:SUB] ...") presence + the tag(s) used
      - hnh_Hook() registration: address + registering register_* function

Heuristic brace/regex scanner tuned to the decompiled style in hnhmods/src
(no templates, no file-scope classes, globals are `T &g_x_ADDR = *(T*)ADDR;`).

Usage:
  py tools/scan_cpp.py hnhmods/src/CheckRunGame_4863C8.cpp
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

# --------------------------------------------------------------------- reading


def read_source(path: Path) -> str:
    data = path.read_bytes()
    for enc in ("utf-8-sig", "utf-8"):
        try:
            return data.decode(enc)
        except UnicodeDecodeError:
            pass
    # mojibake-safe fallback (Shift-JIS remnants in string literals); keeps
    # line numbers intact, which is all the scanner needs.
    return data.decode("latin-1")


def strip_comments(src: str) -> str:
    """Blank out // and /* */ comments. Length- and line-preserving; string
    and char literals are left intact so nested quotes don't confuse it."""
    out = list(src)
    i, n = 0, len(src)
    state = "code"
    while i < n:
        c = src[i]
        nxt = src[i + 1] if i + 1 < n else ""
        if state == "code":
            if c == "/" and nxt == "/":
                out[i] = out[i + 1] = " "
                state, i = "line", i + 2
            elif c == "/" and nxt == "*":
                out[i] = out[i + 1] = " "
                state, i = "block", i + 2
            elif c == '"':
                state, i = "str", i + 1
            elif c == "'":
                state, i = "chr", i + 1
            else:
                i += 1
        elif state in ("str", "chr"):
            quote = '"' if state == "str" else "'"
            if c == "\\":
                i += 2
            elif c == quote:
                state, i = "code", i + 1
            else:
                i += 1
        elif state == "line":
            if c == "\n":
                state = "code"
            else:
                out[i] = " "
            i += 1
        else:  # block comment
            if c == "*" and nxt == "/":
                out[i] = out[i + 1] = " "
                state, i = "code", i + 2
            else:
                if c != "\n":
                    out[i] = " "
                i += 1
    return "".join(out)


# --------------------------------------------------------------------- scanning

TYPE_KEYWORDS = re.compile(
    r"^\s*(struct|class|enum|union|namespace|typedef|using|friend|template|extern)\b"
)
TAG_KEYWORDS = re.compile(r"^(struct|class|enum|union|namespace)\s+([A-Za-z_]\w*)")
FUNC_NAME = re.compile(r"([A-Za-z_]\w*)\s*\(")
NOT_FUNC_NAMES = {
    "if", "for", "while", "switch", "return", "sizeof", "do", "else", "asm",
    "__declspec", "static_assert", "alignas",
}

ARRAY_REF = re.compile(r"\(\s*&\s*([A-Za-z_]\w*)\s*\)")
FUNC_PTR = re.compile(
    r"\(\s*(?:__cdecl\s*|__stdcall\s*|__fastcall\s*|__thiscall\s*|__vectorcall\s*)?"
    r"\*?\s*([A-Za-z_]\w*)\s*\)\s*\("
)
PLAIN_NAME = re.compile(r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\])*$")
ADDR_SUFFIX = re.compile(r"_([0-9A-Fa-f]{6})$")
INIT_ADDR = re.compile(r"0x([0-9A-Fa-f]{6})\b")
HOOK_CALL = re.compile(r"\bhnh_Hook\s*\(")
DEBUG_TAG = re.compile(r'DebugLog\s*\(\s*"\[HNH:SUB\]\s*([^"\\]*)')


def find_matching_brace(text: str, open_pos: int) -> int:
    depth = 0
    i, n = open_pos, len(text)
    while i < n:
        c = text[i]
        if c == '"':
            i += 1
            while i < n:
                if text[i] == "\\":
                    i += 2
                    continue
                if text[i] == '"':
                    break
                i += 1
        elif c == "'":
            i += 1
            while i < n:
                if text[i] == "\\":
                    i += 2
                    continue
                if text[i] == "'":
                    break
                i += 1
        elif c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
        i += 1
    return n - 1


def split_call_args(text: str, open_pos: int) -> list[str]:
    """Split the top-level comma-separated args of a call whose '(' is at open_pos."""
    i, n, depth = open_pos, len(text), 1
    while i < n:
        c = text[i]
        if c == '"':
            i += 1
            while i < n:
                if text[i] == "\\":
                    i += 2
                    continue
                if text[i] == '"':
                    break
                i += 1
        elif c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                break
        i += 1
    inner = text[open_pos:i]
    args, cur, lvl = [], [], 0
    for ch in inner:
        if ch in "([{":
            lvl += 1
        elif ch in ")]}":
            lvl -= 1
        if ch == "," and lvl == 0:
            args.append("".join(cur))
            cur = []
        else:
            cur.append(ch)
    if cur:
        args.append("".join(cur))
    return args


def classify_global(decl: str) -> tuple[str, str | None]:
    """(kind, name) for a top-level `;`-terminated declaration."""
    head = decl.split("=", 1)[0].strip()
    m = ARRAY_REF.search(head)
    if m:
        return "array-ref", m.group(1)
    m = FUNC_PTR.search(head)
    if m:
        return "func-ptr", m.group(1)
    m = PLAIN_NAME.search(head)
    if m:
        return "data", m.group(1)
    return "?", None


def addr_from_name(name: str) -> str:
    m = ADDR_SUFFIX.search(name)
    return "0x" + m.group(1).upper() if m else ""


def debug_tags(body: str) -> list[str]:
    return [m.group(1).strip() for m in DEBUG_TAG.finditer(body)]


def scan(text: str, src: str):
    """Returns (globals, funcs, structs, hooks)."""
    n = len(text)
    src_lines = src.splitlines()

    def line_of(pos: int) -> int:
        return src.count("\n", 0, pos) + 1

    def first_nonspace(a: int, b: int) -> int:
        while a < b and text[a].isspace():
            a += 1
        return a

    globals_: list[dict] = []
    funcs: list[dict] = []
    structs: list[tuple[str, int]] = []

    pos = stmt_start = 0
    while pos < n:
        c = text[pos]
        # preprocessor directive: skip whole logical line
        if c == "#" and (pos == 0 or text[pos - 1] == "\n"):
            j = pos
            while j < n and not (text[j] == "\n" and (j == 0 or text[j - 1] != "\\")):
                j += 1
            pos = stmt_start = min(j + 1, n)
            continue
        if c == "{":
            begin = first_nonspace(stmt_start, pos)
            sig = " ".join(text[begin:pos].split())
            end = find_matching_brace(text, pos)
            m_kw = TAG_KEYWORDS.match(sig)
            if m_kw:
                structs.append((m_kw.group(2), line_of(begin)))
            elif sig and "(" in sig and "=" not in sig:
                m = FUNC_NAME.search(sig)
                if m and m.group(1) not in NOT_FUNC_NAMES:
                    funcs.append({
                        "name": m.group(1),
                        "sig": sig,
                        "line": line_of(begin),
                        "body": text[pos:end + 1],
                        "body_pos": pos,
                    })
            pos = end + 1
            if pos < n and text[pos] == ";":
                pos += 1
            stmt_start = pos
            continue
        if c == ";":
            begin = first_nonspace(stmt_start, pos)
            flat = " ".join(text[begin:pos].split())
            if flat and not TYPE_KEYWORDS.match(flat):
                kind, name = classify_global(flat)
                if name:
                    ln = line_of(begin)
                    note = ""
                    if 0 < ln <= len(src_lines):
                        m = re.search(r"//\s*(.+)$", src_lines[ln - 1])
                        if m:
                            note = m.group(1).strip()
                    globals_.append({
                        "name": name,
                        "kind": kind,
                        "line": ln,
                        # prefer the _ADDR name suffix, fall back to the
                        # address in the `= *(T*)0x...` initializer
                        "addr": addr_from_name(name)
                        or (lambda m: "0x" + m.group(1).upper() if m else "")(
                            INIT_ADDR.search(flat)
                        ),
                        "note": note,
                    })
            pos += 1
            stmt_start = pos
            continue
        pos += 1

    # hnh_Hook registrations, collected from function bodies
    hooks: dict[str, dict] = {}
    for f in funcs:
        for m in HOOK_CALL.finditer(f["body"]):
            args = split_call_args(f["body"], m.end())
            if len(args) < 2:
                continue
            hooks[args[1].strip()] = {
                "addr": args[0].strip(),
                "container": f["name"],
                "line": line_of(f["body_pos"] + m.start()),
            }
    return globals_, funcs, structs, hooks


# --------------------------------------------------------------------- report


def pad(s: str, w: int) -> str:
    s = str(s)
    return s if len(s) <= w else s[: w - 1] + ">"


def report(path: Path) -> None:
    src = read_source(path)
    text = strip_comments(src)
    globals_, funcs, structs, hooks = scan(text, src)

    print("=" * 110)
    print(str(path))
    print("=" * 110)

    print(f"\nSTRUCTS ({len(structs)})")
    for name, ln in structs:
        print(f"  L{ln:<6}{name}")

    print(f"\nGLOBALS ({len(globals_)})")
    nw = max([len(g["name"]) for g in globals_] + [4])
    for g in globals_:
        note = f"  // {g['note']}" if g["note"] else ""
        print(
            f"  L{g['line']:<6}{g['kind']:<10}{pad(g['name'], nw):<{nw}}"
            f"  {g['addr'] or '-':<10}{note}"
        )

    impl = [f for f in funcs if not f["name"].startswith("register_")]
    registrars = [f for f in funcs if f["name"].startswith("register_")]

    print(f"\nFUNCTIONS ({len(funcs)})")
    fw = max([len(f["name"]) for f in funcs] + [4])
    print(f"  {'line':<7}{pad('name', fw):<{fw}}  {'DebugLog [HNH:SUB]':<44}  hnh_Hook")
    for f in funcs:
        tags = debug_tags(f["body"])
        dbg = f"yes: {tags[0]}" + (f" (+{len(tags)-1})" if len(tags) > 1 else "") if tags else "no"
        if f["name"].startswith("register_"):
            cnt = sum(1 for h in hooks.values() if h["container"] == f["name"])
            reg = f"registrar ({cnt} hooks)"
        elif f["name"] in hooks:
            h = hooks[f["name"]]
            reg = f"{h['addr']} in {h['container']} (L{h['line']})"
        else:
            reg = "NOT REGISTERED"
        print(f"  L{f['line']:<6}{pad(f['name'], fw):<{fw}}  {pad(dbg, 44):<44}  {reg}")

    no_dbg = [f for f in impl if not debug_tags(f["body"])]
    hooked = [f for f in impl if f["name"] in hooks]
    no_hook = [f for f in impl if f["name"] not in hooks]

    print("\nSUMMARY")
    print(f"  structs:              {len(structs)}")
    n_fp = sum(1 for g in globals_ if g["kind"] == "func-ptr")
    print(f"  globals:              {len(globals_)} ({n_fp} func-ptr imports)")
    print(f"  functions:            {len(funcs)} ({len(impl)} implemented + {len(registrars)} registrar)")
    print(f"  DebugLog [HNH:SUB]:   {len(impl) - len(no_dbg)}/{len(impl)}")
    print(f"  hnh_Hook-registered:  {len(hooked)}/{len(impl)}")
    if no_dbg:
        print("  missing DebugLog:     " + ", ".join(f["name"] for f in no_dbg))
    if no_hook:
        print("  not registered:       " + ", ".join(f["name"] for f in no_hook))
    defined = {f["name"] for f in funcs}
    ext = {t: h for t, h in hooks.items() if t not in defined}
    if ext:
        print("  hooks to targets not defined in this file:")
        for t, h in ext.items():
            print(f"      {t:<40} {h['addr']} in {h['container']}")


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("files", nargs="+", type=Path, help="C++ source files to scan")
    args = ap.parse_args(argv)
    if sys.stdout and hasattr(sys.stdout, "reconfigure"):
        try:
            sys.stdout.reconfigure(encoding="utf-8", errors="replace")
        except Exception:
            pass
    for i, p in enumerate(args.files):
        if not p.is_file():
            print(f"error: {p} not found", file=sys.stderr)
            return 1
        if i:
            print()
        report(p)
    return 0


if __name__ == "__main__":
    sys.exit(main())
