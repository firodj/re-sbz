#!/usr/bin/env python3
"""
gen_modproj.py -- generate a new <name>mods project from the XYZmods template.

Asks for a short name (e.g. `wew`), copies XYZmods/ to <name>mods/ with every
`XYZ` replaced by the name (folder name, file names and file contents), writes a
fresh ProjectGuid into the new .vcxproj, and registers the project in
re-sbz.sln (unless --no-sln). The XYZmods template itself is never modified.

Exception to the literal replacement: `XYZMODS_EXPORTS` becomes
`<NAME>MODS_EXPORTS` (uppercase) to follow the Visual Studio macro convention.

Usage:
    py tools/gen_modproj.py [name] [--no-sln]
"""

import argparse
import re
import sys
import uuid
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
TEMPLATE_DIR = REPO / "XYZmods"
TEMPLATE_TOKEN = b"XYZ"
EXPORTS_TOKEN = b"XYZMODS_EXPORTS"
SOLUTION = REPO / "re-sbz.sln"

NAME_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
PROJECT_GUID_RE = re.compile(rb"(<ProjectGuid>\{)([0-9A-Fa-f-]{36})(\}</ProjectGuid>)")


def die(msg):
    print(f"error: {msg}", file=sys.stderr)
    sys.exit(1)


def parse_name(raw):
    """Return (prefix, folder): 'wew' -> ('wew', 'wewmods'), 'wewmods' -> ('wew', 'wewmods')."""
    raw = raw.strip()
    if raw.lower().endswith("mods") and len(raw) > len("mods"):
        raw = raw[: -len("mods")]
    if not NAME_RE.match(raw):
        die(f"invalid name {raw!r}: use letters/digits/underscore, starting with a letter or underscore")
    if raw.lower() == "xyz":
        die("name 'xyz' would recreate the XYZmods template")
    return raw, raw + "mods"


def copy_template(dst, prefix):
    """Copy the template tree, replacing XYZ in file names and contents (byte-level)."""
    upper_exports = prefix.upper().encode("ascii") + b"MODS_EXPORTS"
    count = 0
    for src in sorted(TEMPLATE_DIR.rglob("*")):
        if src.is_dir():
            continue
        rel = src.relative_to(TEMPLATE_DIR)
        if any(p.startswith(".") or p == "__pycache__" for p in rel.parts):
            continue
        out = dst.joinpath(*[p.replace("XYZ", prefix) for p in rel.parts])
        out.parent.mkdir(parents=True, exist_ok=True)
        data = src.read_bytes().replace(EXPORTS_TOKEN, upper_exports)
        out.write_bytes(data.replace(TEMPLATE_TOKEN, prefix.encode("ascii")))
        count += 1
    return count


def refresh_project_guid(vcxproj):
    """Give the copied .vcxproj a fresh ProjectGuid; return it as a lowercase string."""
    guid = str(uuid.uuid4())
    data = vcxproj.read_bytes()
    data, n = PROJECT_GUID_RE.subn(rb"\g<1>" + guid.encode("ascii") + rb"\g<3>", data)
    if n != 1:
        die(f"expected 1 ProjectGuid in {vcxproj.name}, found {n}")
    vcxproj.write_bytes(data)
    return guid


def template_project_guid():
    data = (TEMPLATE_DIR / "XYZmods.vcxproj").read_bytes()
    m = PROJECT_GUID_RE.search(data)
    if not m:
        die("cannot find ProjectGuid in XYZmods/XYZmods.vcxproj")
    return m.group(2).decode("ascii")


def add_to_solution(folder, guid, tpl_guid):
    """Clone the XYZmods project block and its config entries under the new name/GUID."""
    lines = SOLUTION.read_bytes().splitlines(keepends=True)
    tpl = b"{" + tpl_guid.upper().encode("ascii") + b"}"
    new = b"{" + guid.upper().encode("ascii") + b"}"
    if new in b"".join(lines):
        die("generated GUID already present in re-sbz.sln")

    start = next((i for i, l in enumerate(lines) if b"XYZmods\\XYZmods.vcxproj" in l), None)
    if start is None:
        die("XYZmods project entry not found in re-sbz.sln")
    end = next((i for i in range(start, len(lines)) if lines[i].strip() == b"EndProject"), None)
    if end is None:
        die("EndProject for XYZmods not found in re-sbz.sln")
    block = [l.replace(b"XYZmods", folder.encode("ascii")).replace(tpl, new) for l in lines[start:end + 1]]

    cfg = [i for i, l in enumerate(lines) if l.lstrip().startswith(tpl + b".")]
    if not cfg:
        die("XYZmods configuration entries not found in re-sbz.sln")
    cfg_lines = [lines[i].replace(tpl, new) for i in cfg]

    # insert the later-in-file config lines first so the block indices stay valid
    lines[cfg[-1] + 1:cfg[-1] + 1] = cfg_lines
    lines[end + 1:end + 1] = block
    SOLUTION.write_bytes(b"".join(lines))


def main():
    ap = argparse.ArgumentParser(description="Generate a new <name>mods project from the XYZmods template.")
    ap.add_argument("name", nargs="?", help="short name, e.g. 'wew' -> creates 'wewmods'")
    ap.add_argument("--no-sln", action="store_true", help="do not register the project in re-sbz.sln")
    args = ap.parse_args()

    if not TEMPLATE_DIR.is_dir():
        die(f"template folder {TEMPLATE_DIR} not found")

    prefix, folder = parse_name(args.name or input("New mod project name (e.g. 'wew' -> 'wewmods'): "))
    dst = REPO / folder
    if dst.exists():
        die(f"{dst} already exists")

    count = copy_template(dst, prefix)
    guid = refresh_project_guid(dst / f"{folder}.vcxproj")
    print(f"created {folder}/ ({count} files), ProjectGuid {{{guid}}}")

    if not args.no_sln:
        add_to_solution(folder, guid, template_project_guid())
        print(f"added {folder} to {SOLUTION.name}")

    print(f"next: set APPDIR/APPEXE in {folder}/PropertySheet.props and pfnWinMain in dllmain.cpp")


if __name__ == "__main__":
    main()
