# re-SBZ

## scan_sj_v2.py

Scans .rdata segment for Shift-JIS encoded strings and creates them in IDA.

## lister.py

Lists all heads in .rdata segment.

## scan_funcs.py

Scans .text segment for function prologues and creates functions in IDA.

## rename_funcs.py

Renames functions in IDA based on patterns.

## miscs.py

Miscellaneous functions that dont depend on IDA API.

# create_class_struct.py

Generic IDA Pro Python script to create a C struct for a class by analyzing
its constructor.

# Notes

Done! Renamed sub_695F00 → EnsureTrailingSlash.

# IDA Plugin

* https://github.com/herosi/PyClassInformer (better than his IDA_ClassInformer)
* https://github.com/mrexodia/ida-pro-mcp (IDA MCP Server)
* https://github.com/oopsmishap/HexRaysPyTools (Structure Builder)

# Formats
* https://github.com/Techokami/noesis-plugins-official
* https://www.richwhitehouse.com/index.php?content=inc_projects.php&filemirror=noesisv4474.zip