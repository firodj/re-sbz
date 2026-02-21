import idaapi
import ida_nalt


def rename_callerapi_for(name, prefix):
    ea = idc.get_name_ea_simple(name)
    if ea == idc.BADADDR:
        print(f"{name} not found in imports.")
        return

    count = 0
    for xref in idautils.XrefsTo(ea, 0):
        #print(f"{xref.frm:x}: {xref.to:x}")
        func = ida_funcs.get_func(xref.frm)
        if func and (func.name.startswith("sub_") or func.name.startswith("unk_")):
            target_name = f"{prefix}_{func.start_ea:X}"
            print(f"Rename func {func.name} at {func.start_ea:X} to {target_name}")
            ida_name.set_name(func.start_ea, target_name, ida_name.SN_NOWARN)
            count += 1
    
    print(f"Rename {count} functions for api {name}")

if __name__ == '__main__':
    rename_callerapi_for("RegQueryValueExA", "RegQuery")
    rename_callerapi_for("RegQueryValueExW", "RegQuery")
