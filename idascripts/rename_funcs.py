import idautils
import ida_bytes
import idc
import ida_name
import ida_funcs
import importlib
import lister
importlib.reload(lister)
from lister import yara_to_regex

def hexstring_to_offset(hexstr):
    return int.from_bytes(bytes.fromhex(hexstr), byteorder='little')
    
def disp_to_offset(hexstring):
    if hexstring[0] == '0':
        return 0
    elif hexstring[0] in ('4', '8'):
        return hexstring_to_offset(hexstring[2:])

    print("Unknown offset type: " + hexstring)
    return None

def disp_to_offset_2(hexstring):
    if len(hexstring) == 0:
        return 0
    elif hexstring[0:2] == '83':
        return hexstring_to_offset(hexstring[4:])
    elif hexstring[0:2] == '05':
        return hexstring_to_offset(hexstring[2:])

    print("Unknown offset type: " + hexstring)
    return None
    
def rename_functions():
    patterns = [
        {
            "name": lambda m: f"Get_Member_{disp_to_offset(m.group(1)):X}_Val",
            "yara": "55 8b ec 51 89 4d fc 8b 45 fc 8b ( 00 | 40 ?? | 80 ?? ?? ?? ?? ) 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc 8b",
            "description": "Returns [this+X]"
        },
        {
            "name": lambda m: f"Get_Member_{disp_to_offset(m.group(1)):X}_Val",
            "yara": "55 8b ec 51 89 4d fc 8b 45 fc 8a ( 00 | 40 ?? | 80 ?? ?? ?? ?? ) 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc 8b",
            "description": "Returns [this+X] as Byte"
        },
        {
            "name": lambda m: f"Empty_Member_Func",
            "yara": "55 8b ec 51 89 4d fc 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc 8b",
            "description": "Empty member function"
        },
        {
            "name": lambda m: f"Empty_Member_Func",
            "yara": "55 8b ec 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 5d",
            "description": "Empty member function"
        },
        {
            "name": lambda m: f"Get_Elem_{disp_to_offset_2(m.group(1)):X}_Val",
            "yara": "55 8b ec 51 89 4d fc 8b 45 fc ( 83 c0 ?? | 05 ?? ?? ?? ?? | ) 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc 8b",
            "description": "Returns this pointer"
        },
        {
            "name": lambda m: f"Get_True",
            "yara": "55 8b ec 51 89 4d fc b0 01 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc b0",
            "description": "Returns 1"
        },
        {
            "name": lambda m: f"Get_False",
            "yara": "55 8b ec 51 89 4d fc 33 c0 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc 33",
            "description": "Returns 0"
        },
        {
            "name": lambda m: f"Set_Member_{disp_to_offset(m.group(1)):X}_Val",
            "yara": "55 8b ec 51 89 4d fc 8b 45 fc 8b 4d 08 89 ( 08 | 48 ?? | 88 ?? ?? ?? ?? ) ( 8b 45 fc | ) 8b e5 5d ( c3 | c2 ?? ?? )",
            "prefix": "55 8b ec 51 89 4d fc 8b 45 fc 8b 4d 08",
            "description": "Set [this+X] Then Returns this"
        },
    ]

    print("Starting function rename scan...")
    
    total_renamed = 0

    for pattern in patterns:
        rex = yara_to_regex(pattern["yara"])
        prefix = bytes.fromhex(pattern["prefix"])
        prefix_len = len(prefix)
        pattern_count = 0
        
        print(f"Scanning for pattern: {pattern['description']}...")

        for func_ea in idautils.Functions():
            fun = ida_funcs.get_func(func_ea)
            # Quick check first
            if ida_bytes.get_bytes(func_ea, prefix_len) != prefix:
                continue

            # Full check
            func_bytes = ida_bytes.get_bytes(func_ea, fun.end_ea - fun.start_ea)
            
            m = rex.match(func_bytes.hex())
            if m:
                old_name = idc.get_name(func_ea)
                new_name = pattern['name'](m)
                new_name = f"{new_name}_{func_ea:X}"
                
                # Rename if not already named correctly or if it's a generic/auto-generated name
                # We check for sub_, unknown_, or previous incomplete renames
                if old_name != new_name and (
                    old_name.startswith("sub_") or 
                    old_name.startswith("unknown_") or 
                    "@Concurrency@" in old_name):
                    wet_run = ida_name.set_name(func_ea, new_name, ida_name.SN_NOWARN)
                    if wet_run:
                        print(f"  Renamed {hex(func_ea)}: {old_name} -> {new_name}")
                        pattern_count += 1
                        total_renamed += 1
                    else:
                        print(f"  Failed to rename {hex(func_ea)}")
                elif old_name == new_name:
                    # Already renamed
                    # print(f"  Skipping {hex(func_ea)}, already renamed.")
                    pass
        
        print(f"  Found and processed {pattern_count} matches for {pattern['description']}")

    print(f"Scan complete. Total functions renamed: {total_renamed}")

if __name__ == "__main__":
    rename_functions()
