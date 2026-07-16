import ida_bytes
import idc
import ida_funcs
import ida_name
import re

def check_offset_target_1(ea):

    flags = ida_bytes.get_flags(ea)
    
    # 1. Ensure it is a data offset
    if not ida_bytes.is_data(flags) or not ida_bytes.is_off0(flags):
        print(f"[-] {hex(ea)} is not a defined data offset pointer.")
        return
        
    size = ida_bytes.get_item_size(ea)
    
    # 2. Read the pointer value (the target address)
    if size == 4:
        target_ea = idc.get_wide_dword(ea)
    elif size == 8:
        target_ea = idc.get_wide_qword(ea)
    else:
        print(f"[-] Unexpected data size ({size} bytes) at {hex(ea)}.")
        return None

    #print(f"[+] Pointer at {hex(ea)} points to target address: {hex(target_ea)}")

    # 3. Check if the target is mapped memory
    if not ida_bytes.is_mapped(target_ea):
        print(f"    [-] Target address {hex(target_ea)} points to unmapped/invalid memory.")
        return None

    # 4. Check if the target is a function or code
    func = ida_funcs.get_func(target_ea)
    if func:
        # Get the true name of the function start
        func_name = idc.get_name(func.start_ea)
        demangled = idc.demangle_name(func_name, idc.get_inf_attr(idc.INF_SHORT_DN))
        
        display_name = demangled if demangled else func_name
        #print(f"    [!] Target is a FUNCTION: {display_name} (Starts at {hex(func.start_ea)})")
        return ("func", display_name, func.start_ea)
    
    elif ida_bytes.is_code(ida_bytes.get_flags(target_ea)):
        # It's code, but IDA hasn't marked it as a function chunk yet
        loc_name = idc.get_name(target_ea)
        #print(f"    [!] Target is CODE/INSTRUCTION. Name/Label: {loc_name if loc_name else 'None'}")
        return ("code", loc_name if loc_name else 'None')
        
    else:
        # It points to data, another offset, or a string
        target_name = idc.get_name(target_ea)
        #print(f"    [.] Target is DATA. Name/Label: {target_name if target_name else 'None'}")
        return ("data", target_name if target_name else 'None')

def main():
    ea = idc.here()
    prev_ea = ea    

    is_find_frist = False
    step_back = 5
    fail = True

    while True:
        info = check_offset_target_1(ea)
        if info is None or info[0] != 'func':
            if is_find_frist:
                ea = prev_ea
                fail = False
            break

        is_find_frist = True
        prev_ea = ea
        ea = ea - 4
        step_back -= 1
        if step_back <= 0:
            break

    if fail:
        return False

    step_forward = 50
    base_ea = ea
    while True:
        info = check_offset_target_1(ea)
        if info is None or info[0] != 'func':
            break

        #print(ea, info)
        s = info[1]
        start_ea = info[2]
        new_name = re.sub(r'(.)sub_', r'\1tod_' + f'{ea-base_ea:X}_', s)
        
        # Apply the name
        # SN_NOWARN prevents IDA from popping up warning dialogs if the name is a duplicate
        success = ida_name.set_name(start_ea, new_name, ida_name.SN_NOWARN)

        if success:
            print(f"[+] Successfully renamed {hex(ea)} to {new_name}")
        else:
            print(f"[-] Failed to rename {hex(ea)}. The name might be invalid or already taken.")

        ea += 4
        step_forward -= 1
        if step_forward <= 0:
            break


if __name__ == '__main__':
    main()
