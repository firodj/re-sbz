import idc, idaapi, idautils

def set_file_api_breakpoints():
    print("Setting breakpoints on file access APIs...")
    
    # Define the target imported functions
    targets = ["CreateFileA", "CreateFileW", "ReadFile"]
    bpt_count = 0
    
    # Iterate through all imported modules
    nimps = idaapi.get_import_module_qty()
    for i in range(nimps):
        module_name = idaapi.get_import_module_name(i)
        if not module_name:
            continue
            
        def imp_cb(ea, name, ordinal):
            nonlocal bpt_count
            if name in targets:
                # Find all cross-references to the IAT entry 
                # (these are the 'call' or 'jmp' stubs throughout the code)
                stubs = list(idautils.CodeRefsTo(ea, 0))
                
                if stubs:
                    for stub in stubs:
                        # Set a standard execution breakpoint
                        idc.add_bpt(stub, 1, idc.BPT_DEFAULT)
                        print(f"Set BPT on {name} stub at 0x{stub:X}")
                        bpt_count += 1
                else:
                    # Fallback: Set a breakpoint on the IAT entry directly
                    idc.add_bpt(ea, 1, idc.BPT_DEFAULT)
                    print(f"Set BPT on {name} IAT entry at 0x{ea:X}")
                    bpt_count += 1
            return True

        # Enumerate imports in the current module
        idaapi.enum_import_names(i, imp_cb)
        
    print(f"Done. Set {bpt_count} breakpoints total.")

if __name__ == "__main__":
    set_file_api_breakpoints()
