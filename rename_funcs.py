import idautils
import ida_bytes
import idc
import ida_name
import ida_funcs
import importlib
import lister
importlib.reload(lister)
from lister import yara_to_regex

def rename_functions():
    patterns = [
        {
            "name": "Get_First_Member_Val",
            "yara": "55 8b ec 51 89 4d fc 8b 45 fc 8b 00 8b e5 5d c3",
            "prefix": "55 8b ec 51 89 4d fc 8b",
            "description": "Returns [this+0]"
        },
        {
            "name": "Empty_Member_Func",
            "yara": "55 8b ec 51 89 4d fc 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc 8b",
            "description": "Empty member function with retn"
        },
    ]

    print("Starting function rename scan...")
    
    total_renamed = 0

    for pattern in patterns:
        rex = yara_to_regex(pattern["yara"])
        prefix = bytes.fromhex(pattern["prefix"])
        prefix_len = len(prefix)
        pattern_count = 0
        
        print(f"Scanning for pattern: {pattern['name']}...")

        for func_ea in idautils.Functions():
            fun = ida_funcs.get_func(func_ea)
            # Quick check first
            if ida_bytes.get_bytes(func_ea, prefix_len) != prefix:
                continue

            # Full check
            func_bytes = ida_bytes.get_bytes(func_ea, fun.end_ea - fun.start_ea)
            
            if rex.match(func_bytes.hex()):
                old_name = idc.get_name(func_ea)
                new_name = f"{pattern['name']}_{hex(func_ea)[2:].upper()}"
                
                # Rename if not already named correctly or if it's a generic/auto-generated name
                # We check for sub_, unknown_, or previous incomplete renames
                if old_name != new_name and (old_name.startswith("sub_") or old_name.startswith("unknown_") or pattern['name'] in old_name):
                    if ida_name.set_name(func_ea, new_name, ida_name.SN_NOWARN):
                        print(f"  Renamed {hex(func_ea)}: {old_name} -> {new_name}")
                        pattern_count += 1
                        total_renamed += 1
                    else:
                        print(f"  Failed to rename {hex(func_ea)}")
                elif old_name == new_name:
                    # Already renamed
                    # print(f"  Skipping {hex(func_ea)}, already renamed.")
                    pass
        
        print(f"  Found and processed {pattern_count} matches for {pattern['name']}")

    print(f"Scan complete. Total functions renamed: {total_renamed}")

if __name__ == "__main__":
    rename_functions()
