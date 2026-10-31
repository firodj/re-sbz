import idautils
import ida_funcs
import ida_hexrays
import ida_typeinf

TARGET_TYPE_NAME = "MenuState"  # Replace with the type you are looking for, e.g., "int" or a struct name

def check_type_str(tinfo):
    if tinfo.empty():
        return False
    # Get the type name or string representation
    type_str = ida_typeinf.print_tinfo('', 0, 0, ida_typeinf.PRTYPE_1LINE, tinfo, '', '')
    return TARGET_TYPE_NAME in type_str

def scan_functions():
    if not ida_hexrays.init_hexrays_plugin():
        print("Hex-Rays decompiler is not available.")
        return

    print(f"Scanning functions for type matching: {TARGET_TYPE_NAME}")
    
    for func_ea in idautils.Functions():
        f = ida_funcs.get_func(func_ea)
        if not f:
            continue
            
        func_name = ida_funcs.get_func_name(func_ea)
        
        # Try to get the decompiler Ctree/lvars
        try:
            cfunc = ida_hexrays.decompile(func_ea)
            if not cfunc:
                continue
            
            # Check local variables and arguments in the decompiler view
            lvars = cfunc.get_lvars()
            for lvar in lvars:
                if check_type_str(lvar.type()):
                    print(f"[+] Function {func_name} (0x{func_ea:x}) has var/arg '{lvar.name}' of type {lvar.type()}")
                    break
        except Exception as e:
            pass

if __name__ == "__main__":
    scan_functions()
