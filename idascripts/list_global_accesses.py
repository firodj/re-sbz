# List all accesses to global variables in the current function
# Compatible with IDA Pro 9.1

import ida_segment
import idaapi
import idautils
import ida_funcs
import ida_name
import ida_lines
import ida_xref

def list_global_accesses_in_func():
    # Get current address (where cursor is)
    here = idaapi.get_screen_ea()
    if here == idaapi.BADADDR:
        print("Invalid address")
        return
        
    # Get function containing the current address
    func = ida_funcs.get_func(here)
    if func is None:
        print("Current address is not in a function")
        return

    start_ea = func.start_ea
    end_ea = func.end_ea
    func_name = ida_funcs.get_func_name(here) or "<no name>"
    print(f"Function: {func_name} (0x{start_ea:X} - 0x{end_ea:X})")
    print("Accesses to global variables:")

    count = 0
    # Iterate over each instruction in the function
    for ea in idautils.FuncItems(start_ea):
        # Get data references from this instruction
        # XrefsFrom with XREF_DATA flag gives only data references
        refs = idautils.XrefsFrom(ea, ida_xref.XREF_DATA)
        if refs:
            for ref in refs:
                ref_ea = ref.to  # The address being referenced
                seg = ida_segment.getseg(ref_ea)
                if seg is None:
                    continue
                
                # Check if segment is likely a data segment
                # Method 1: Check segment class (if available)
                # Method 2: Check segment name for common data segments
                seg_name = ida_segment.get_segm_name(seg)
                is_data_seg = False
                
                # Check by segment class if the function exists
                try:
                    seg_class = ida_segment.get_segm_class(seg)
                    if seg_class == ida_segment.SEG_DATA:
                        is_data_seg = True
                except AttributeError:
                    pass  # Function doesn't exist, try other methods
                
                # Check by segment name
                if not is_data_seg:
                    seg_name_lower = seg_name.lower()
                    if any(data_seg in seg_name_lower for data_seg in ['.data', '.rdata', '.bss', '.sbss', '.sdata']):
                        is_data_seg = True
                
                # Alternative: check if not code segment
                if not is_data_seg:
                    try:
                        seg_class = ida_segment.get_segm_class(seg)
                        if seg_class != ida_segment.SEG_CODE:
                            is_data_seg = True
                    except AttributeError:
                        # If we can't determine, assume it's data if not obviously code
                        if 'code' not in seg_name_lower and 'text' not in seg_name_lower:
                            is_data_seg = True
                
                if is_data_seg:
                    count += 1
                    name = ida_name.get_name(ref_ea)
                    if name == "":
                        name = "<no name>"
                    disasm = ida_lines.generate_disasm_line(ea, 0)
                    # print(f"0x{ea:X}: {disasm} -> accesses global variable 0x{ref_ea:X} ({name})")
                    print(f"0x{ea:X}: 0x{ref_ea:X} ({name})")

    if count == 0:
        print("No accesses to global variables found in the current function.")

if __name__ == '__main__':
    list_global_accesses_in_func()