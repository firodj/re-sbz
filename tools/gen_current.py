#!/usr/bin/env python3
"""
gen_current.py - cross-reference hnhmods/GOALS.md (call tree) against
hnhmods/src/*.cpp and write hnhmods/CURRENT.md.

For every function in the GOALS tree, marks whether hnhmods/src has:
  [HOOK+PTR file]  detour implemented+registered (hnh_Hook) AND original
                   pinned as a func-ptr global
  [HOOK file]      detour implemented+registered only
  [PTR file]       original pinned as func-ptr only (called, not detoured)
  [IMPL file]      implemented but not hnh_Hook-registered
  [todo]           nothing in hnhmods/src yet

Matching is by address first, then by normalized name ('::' -> '__',
leading 'hook_' stripped). Per-file scan_cpp.py summaries are appended.

Usage:
  py tools/gen_current.py           # hnhmods/GOALS.md -> hnhmods/CURRENT.md
"""

from __future__ import annotations

import argparse
import contextlib
import datetime
import io
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import scan_cpp as sc  # noqa: E402

REPO = Path(__file__).resolve().parent.parent

NODE_RE = re.compile(
    r"^(?P<pre>.*?)(?P<addr>0x[0-9A-Fa-f]{6,8})\s+(?P<name>[A-Za-z_][\w:]*)\s*$"
)


def addr_key(s: str):
    s = s.strip()
    if s.lower().startswith("0x"):
        try:
            return int(s, 16)
        except ValueError:
            return None
    return None


def norm(name: str) -> str:
    """GOALS / source name normalization for the fallback name match."""
    n = name.replace("::", "__")
    return n[5:] if n.startswith("hook_") else n


class Index:
    def __init__(self):
        self.hook_addr: dict[int, tuple] = {}   # addr -> (file, hook target)
        self.hook_name: dict[str, tuple] = {}
        self.ptr_addr: dict[int, tuple] = {}    # addr -> (file, func-ptr name)
        self.ptr_name: dict[str, tuple] = {}
        self.impl_name: dict[str, tuple] = {}   # name -> (file, fn, line)
        self.files: list[Path] = []


def build_index(src_dir: Path) -> Index:
    ix = Index()
    for path in sorted(src_dir.glob("*.cpp")):
        src = sc.read_source(path)
        text = sc.strip_comments(src)
        globals_, funcs, _structs, hooks = sc.scan(text, src)
        ix.files.append(path)

        for target, h in hooks.items():
            entry = (path.name, target)
            a = addr_key(h["addr"])
            if a is not None:
                ix.hook_addr.setdefault(a, entry)
            ix.hook_name.setdefault(norm(target), entry)

        for g in globals_:
            if g["kind"] != "func-ptr":
                continue
            entry = (path.name, g["name"])
            a = addr_key(g["addr"] or "")
            if a is not None:
                ix.ptr_addr.setdefault(a, entry)
            ix.ptr_name.setdefault(norm(g["name"]), entry)

        for f in funcs:
            if f["name"].startswith("register_"):
                continue
            ix.impl_name.setdefault(
                norm(f["name"]), (path.name, f["name"], f["line"])
            )
    return ix


def mark_node(ix: Index, addr: str, name: str):
    """(marker, hooked, pinned) for one GOALS node."""
    a = addr_key(addr)
    hk = (ix.hook_addr.get(a) if a is not None else None) or ix.hook_name.get(norm(name))
    ptr = (ix.ptr_addr.get(a) if a is not None else None) or ix.ptr_name.get(norm(name))
    if hk and ptr:
        return f"[HOOK+PTR {hk[0]}]", hk, ptr
    if hk:
        return f"[HOOK {hk[0]}]", hk, None
    if ptr:
        return f"[PTR {ptr[0]}]", None, ptr
    imp = ix.impl_name.get(norm(name))
    if imp:
        return f"[IMPL {imp[0]}]", None, None
    return "[todo]", None, None


def goals_tree_lines(goals: Path) -> list[str]:
    lines = goals.read_text(encoding="utf-8", errors="replace").splitlines()
    fences = [i for i, l in enumerate(lines) if l.strip() == "```"]
    if len(fences) >= 2:
        return lines[fences[0] + 1: fences[1]]
    return lines


def render_tree(lines: list[str], ix: Index):
    stats = dict(total=0, both=0, hook=0, ptr=0, impl=0, todo=0)
    rendered: list[tuple[str, str | None]] = []
    for line in lines:
        m = NODE_RE.match(line)
        if not m:
            rendered.append((line, None))
            continue
        tag, hk, ptr = mark_node(ix, m["addr"], m["name"])
        rendered.append((line, tag))
        stats["total"] += 1
        if hk and ptr:
            stats["both"] += 1
        elif hk:
            stats["hook"] += 1
        elif ptr:
            stats["ptr"] += 1
        elif tag.startswith("[IMPL"):
            stats["impl"] += 1
        else:
            stats["todo"] += 1
    width = max((len(l.rstrip()) for l, t in rendered if t), default=0) + 2
    out = []
    for l, t in rendered:
        out.append(f"{l.rstrip():<{width}}<- {t}" if t else l.rstrip())
    return out, stats


def file_summary(path: Path) -> str:
    """scan_cpp.py SUMMARY block for one file."""
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        sc.report(path)
    txt = buf.getvalue()
    if "SUMMARY" in txt:
        return "SUMMARY\n" + txt.split("SUMMARY", 1)[1].strip()
    return txt.strip()


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    ap.add_argument("--goals", type=Path, default=REPO / "hnhmods" / "GOALS.md")
    ap.add_argument("--src", type=Path, default=REPO / "hnhmods" / "src")
    ap.add_argument("--out", type=Path, default=REPO / "hnhmods" / "CURRENT.md")
    args = ap.parse_args(argv)
    if sys.stdout and hasattr(sys.stdout, "reconfigure"):
        try:
            sys.stdout.reconfigure(encoding="utf-8", errors="replace")
        except Exception:
            pass

    if not args.goals.is_file():
        print(f"error: {args.goals} not found", file=sys.stderr)
        return 1
    ix = build_index(args.src)
    tree, stats = render_tree(goals_tree_lines(args.goals), ix)
    today = datetime.date.today().isoformat()

    doc = [
        "# CURRENT - GOALS coverage in hnhmods/src",
        "",
        f"Generated {today} by `py tools/gen_current.py` from `hnhmods/GOALS.md`",
        "vs `hnhmods/src/*.cpp` (function data via `tools/scan_cpp.py`).",
        "",
        "Legend:",
        "",
        "- `[HOOK+PTR file]` - detour implemented+registered (`hnh_Hook`) and original pinned as func-ptr",
        "- `[HOOK file]` - detour implemented+registered only",
        "- `[PTR file]` - original pinned as func-ptr only (called, not detoured)",
        "- `[IMPL file]` - implemented but not `hnh_Hook`-registered",
        "- `[todo]` - nothing in hnhmods/src yet",
        "",
        "## Call tree with status",
        "",
        "```",
        *tree,
        "```",
        "",
        "## Stats",
        "",
        f"- tree functions:      {stats['total']}",
        f"- hooked:              {stats['hook'] + stats['both']}"
        f" (HOOK or HOOK+PTR)",
        f"- pinned:              {stats['ptr'] + stats['both']}"
        f" (PTR or HOOK+PTR)",
        f"- fully covered:       {stats['both']} (HOOK+PTR)",
        f"- implemented only:    {stats['impl']}",
        f"- untouched:           {stats['todo']}",
        "",
    ]

    doc += ["## Per-file status (scan_cpp.py)", ""]
    for p in ix.files:
        doc += [f"### hnhmods/src/{p.name}", "", "```", file_summary(p), "```", ""]

    args.out.write_text("\n".join(doc) + "\n", encoding="utf-8")

    print(f"wrote {args.out}")
    print(f"  tree functions: {stats['total']}")
    print(f"  hooked:         {stats['hook'] + stats['both']}")
    print(f"  pinned:         {stats['ptr'] + stats['both']}")
    print(f"  fully covered:  {stats['both']}")
    print(f"  implemented:    {stats['impl']}")
    print(f"  untouched:      {stats['todo']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
