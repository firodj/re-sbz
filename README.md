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

## list_subfuncs2.py

Draw Tree of Functions Callees Recursively from current function at cursor.

## list_global_accesses.py

List Global variables in current function at cursor.

## create_class_struct.py

Generic IDA Pro Python script to create a C struct for a class by analyzing
its constructor.

## analyz_struc.py

Used in class constructor with thiscall (ecx) to auto create instnace struct type.

## analyz_vtabl.py

Auto rename the func name `tod_` prefix instead `sub_` for vmethods, in vtable data.

## tools/gen_current.py

Update `hnhmods/CURRENT.md` to reach `hnhmods/GOALS.md`.

# Notes

Done! Renamed sub_695F00 → EnsureTrailingSlash.

# IDA Plugin

* https://github.com/herosi/PyClassInformer (better than his IDA_ClassInformer)
* https://github.com/mrexodia/ida-pro-mcp (IDA MCP Server)
* https://github.com/oopsmishap/HexRaysPyTools (Structure Builder)

# Formats
* https://github.com/Techokami/noesis-plugins-official
* https://www.richwhitehouse.com/index.php?content=inc_projects.php&filemirror=noesisv4474.zip