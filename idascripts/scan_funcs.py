import ida_funcs
import ida_bytes
import ida_ua
import ida_segment
import idc

def scan_and_define_functions():
    seg = ida_segment.get_segm_by_name(".text")
    if not seg:
        print("Could not find .text segment")
        return

    start_ea = seg.start_ea
    end_ea = seg.end_ea

    created_funcs = 0
    current_ea = start_ea

    print(f"Scanning .text from {hex(start_ea)} to {hex(end_ea)}...")

    while current_ea < end_ea:
        flags = ida_bytes.get_flags(current_ea)
        
        # Check if we are inside an existing function
        func = ida_funcs.get_func(current_ea)
        if func:
            # Skip to end of function
            current_ea = func.end_ea
            continue

        # Case 1: Existing Code but no function
        if ida_bytes.is_code(flags):
            if ida_funcs.add_func(current_ea):
                print(f"Created function at {hex(current_ea)} (was code)")
                created_funcs += 1
                func = ida_funcs.get_func(current_ea)
                if func:
                    current_ea = func.end_ea
                    continue
            else:
                 # Just advance slightly if we can't make a function here
                 current_ea = ida_bytes.next_head(current_ea, end_ea)

        # Case 2: Undefined Data (potential prologue)
        else:
            # Check for push ebp; mov ebp, esp (55 8B EC)
            b1 = ida_bytes.get_byte(current_ea)
            b2 = ida_bytes.get_byte(current_ea + 1)
            b3 = ida_bytes.get_byte(current_ea + 2)
            
            if b1 == 0x55 and b2 == 0x8B and b3 == 0xEC:
                 if ida_ua.create_insn(current_ea):
                     if ida_funcs.add_func(current_ea):
                         print(f"Created function at {hex(current_ea)} (found prologue 55 8B EC)")
                         created_funcs += 1
                         func = ida_funcs.get_func(current_ea)
                         if func:
                            current_ea = func.end_ea
                            continue
            
            # Check for push ebp; mov ebp, esp (55 89 E5 - GCC)
            if b1 == 0x55 and b2 == 0x89 and b3 == 0xE5:
                 if ida_ua.create_insn(current_ea):
                     if ida_funcs.add_func(current_ea):
                         print(f"Created function at {hex(current_ea)} (found prologue 55 89 E5)")
                         created_funcs += 1
                         func = ida_funcs.get_func(current_ea)
                         if func:
                            current_ea = func.end_ea
                            continue

            # If not matched, move to next byte
            current_ea += 1

    print(f"Scan complete. Created {created_funcs} new functions.")

if __name__ == "__main__":
    scan_and_define_functions()
