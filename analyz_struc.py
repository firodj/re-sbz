from bonus import mshtml_find_element_obj
import ida_bytes
import ida_ua
import idautils
import idc
import ida_idp
import ida_funcs
import ida_typeinf
import ida_nalt
import ida_frame

dtype_to_size = {
    ida_ua.dt_byte: (1, idc.FF_BYTE),   # Byte
    ida_ua.dt_word: (2, idc.FF_WORD),   # Word
    ida_ua.dt_dword: (4, idc.FF_DWORD),  # Dword / 4-bytes
    ida_ua.dt_float: (4, idc.FF_FLOAT),  # Float / 4-bytes
    ida_ua.dt_qword: (8, idc.FF_QWORD),  # Qword / 8-bytes
    ida_ua.dt_double: (8, idc.FF_DOUBLE), # Double / 8-bytes
    ida_ua.dt_byte16: (16, idc.FF_OWORD), # Oword / 16-bytes
    ida_ua.dt_byte32: (32, None), # Yword / 32-bytes / DWORD*4
    ida_ua.dt_byte64: (64, None), # Zword / 64-bytes / DWORD*8
}

def analyze_struct_offsets(start_ea, target_reg_name="ecx", this_type: ida_typeinf.tinfo_t = None):
    # 1. Extract this type
    # Get the exact UDT structure details to find its members name
            
    udt_details = ida_typeinf.udt_type_data_t()
    if not this_type.get_udt_details(udt_details):
        print("[-] Error: fail to get udt details")
        return
    struct_id = this_type.get_tid()
    if struct_id != idc.BADADDR:
        print(f"Structure ID: {struct_id}")
    for a in range(len(udt_details)):
        print(udt_details[a].name)


    # 2. Find the function containing the cursor
    func = ida_funcs.get_func(start_ea)
    if not func:
        print("[-] Error: Cursor is not inside a function.")
        return

    start_ea = func.start_ea

    # 3. Flow-chart the function to find basic blocks
    flowchart = ida_gdl.qflow_chart_t("", func, func.start_ea, func.end_ea, 0)
    
    # 4. Find the specific basic block containing 'here()'
    current_block = None
    for block in flowchart:
        if block.start_ea <= start_ea < block.end_ea:
            current_block = block
            break
            
    if not current_block:
        print("[-] Error: Could not isolate the current basic block.")
        return
    
    # get function signature
    # tif = ida_typeinf.tinfo_t()
    # if not ida_frame.get_func_frame(tif, func):
    #     print("[-] Fail to get function frame")
    #     return

    end_ea = current_block.end_ea
    current_ea = start_ea
    var_this = None
    tracked_regs = set(target_reg_name)

    print(f"--- Analyzing offsets for base register '{target_reg_name}' from {start_ea:X} to {end_ea:X} ---")
    
    while current_ea < end_ea:
        # Decode the instruction at the current address
        insn = ida_ua.insn_t()
        if not ida_ua.decode_insn(insn, current_ea):
            current_ea = idc.next_head(current_ea)
            continue

        # Check if ebp is the first operand (destination)
        if insn.get_canon_mnem() not in ("cmp", "test"):
            reg_stack = ida_idp.get_reg_name(insn.Op1.reg, 4).lower()
            if reg_stack in ("ebp"):
                print(f"[{current_ea:X}] ebp modified")

        # 1. Track register assignments to the stack variable
        # e.g., mov [ebp+var_4],ecx
        if insn.get_canon_mnem() == "mov" and insn.Op1.type == ida_ua.o_displ:
            # Check if Op1 is pointing to your stack variable
            reg_stack = ida_idp.get_reg_name(insn.Op1.reg, 4).lower()
            if reg_stack in ("ebp", "esp"):
                # if insn.Op1.addr matches your local variable offset:
                reg_src = ida_idp.get_reg_name(insn.Op2.reg, 4).lower()
                if reg_src == target_reg_name and var_this is None:
                    var_this = (reg_stack, insn.Op1.addr)
                    print(f"[{current_ea:X}] found var_this: {target_reg_name} -> {var_this[0]} + {var_this[1]:X}")

        if insn.get_canon_mnem() == "add" and insn.Op1.type == ida_ua.o_reg:
            reg_dst = ida_idp.get_reg_name(insn.Op1.reg, 4).lower()
            if reg_dst in tracked_regs:
                if insn.Op2.type == ida_ua.o_imm:
                    offset = insn.Op2.value
                    disasm = idc.generate_disasm_line(current_ea, 0)
                    width = dtype_to_size.get(ida_ua.dt_dword) # force call to this is 4
                    print(f"0x{current_ea:08X}: {disasm:<40} -> Found offset: 0x{offset:X} size:{width[0]}")

                    error = idc.add_struc_member(
                        struct_id,
                        f"field_{offset:X}",
                        offset,
                        width[1] | idc.FF_DATA,
                        -1,
                        width[0],
                    )

        # 2.a. Remove tracked register first
        if insn.get_canon_mnem() == "mov" and insn.Op1.type == ida_ua.o_reg:
            reg_dst = ida_idp.get_reg_name(insn.Op1.reg, 4).lower()
            if reg_dst in tracked_regs:
                tracked_regs.remove(reg_dst)
                print(f"[{current_ea:X}] remove tracked regs: {reg_dst}")

        # 2.b. Track register assigments from the stack variable
        # e.g., mov eax, [ebp+var_4]
        if insn.get_canon_mnem() == "mov" and insn.Op2.type == ida_ua.o_displ:
            # Check if Op2 is pointing to your stack variable
            reg_stack = ida_idp.get_reg_name(insn.Op2.reg, 4).lower()
            if reg_stack in ("ebp", "esp") and var_this is not None:
                if var_this[0] == reg_stack and var_this[1] == insn.Op2.addr:
                    # if insn.Op2.addr matches your local variable offset:
                    reg_dst = ida_idp.get_reg_name(insn.Op1.reg, 4).lower()
                    tracked_regs.add(reg_dst)
                    print(f"[{current_ea:X}] add tracked regs: {reg_dst}")

        # Iterate through the instruction's operands (usually up to 2 or 3)
        for op_idx, op in enumerate(insn.ops):
            # o_displ: Memory Reg + Displacement (e.g., [ecx + 30h])
            # o_phrase: Memory Reg + Reg (though often used interchangeably depending on CPU)
            if op.type in (ida_ua.o_displ, ida_ua.o_phrase):
                
                # Get the name of the base register used in the operand
                # op.reg holds the ID of the register (e.g., ecx)
                reg_name = ida_idp.get_reg_name(op.reg, 4).lower() # 4 bytes width for 32-bit registers

                if reg_name in tracked_regs:
                #if reg_name and reg_name.lower() == target_reg_name.lower():
                    # op.addr holds the actual displacement value (the offset)
                    offset = op.addr
                    disasm = idc.generate_disasm_line(current_ea, 0)
                    width = dtype_to_size.get(op.dtype, (0, None))
                    print(f"0x{current_ea:08X}: {disasm:<40} -> Found offset: 0x{offset:X} size:{width[0]}")

                    error = idc.add_struc_member(
                        struct_id,
                        f"field_{offset:X}",
                        offset,
                        width[1] | idc.FF_DATA,
                        -1,
                        width[0],
                    )

                    # add     ecx, 80h

                    # new_member = ida_typeinf.udm_t()
                    # new_member.name = f"field_{offset:X}"
                    # new_member.type = ida_typeinf.tinfo_t(width[1]) # or use parse_decl
                    # new_member.offset = offset # Optional: specify explicit offset

                    # 4. Add member to the container and re-create the type
                    #udt_details.push_back(new_member)
                    #print(f"add new member {new_member}")

                    #break # Move to next instruction once found
                    
        current_ea = idc.next_head(current_ea)
    
    for a in range(len(udt_details)):
        print(udt_details[a].name)

    struct_name = this_type.get_type_name()

    print("--- Done ---")

def check_func_args_struct(ea):
    ea = ida_funcs.get_func(ea).start_ea

    # Get the function signature
    tif = ida_typeinf.tinfo_t()
    if not ida_nalt.get_tinfo(tif, ea):
        print(f"No type information found at {hex(ea)}")
        return

    # Verify it's actually a function
    if not tif.is_func():
        print("This address does not point to a function")
        return

    # Extract function details
    func_details = ida_typeinf.func_type_data_t()
    if not tif.get_func_details(func_details):
        print("Failed to get function details")
        return

    # Check if the calling convention matches CM_CC_THISCALL
    if func_details.cc == ida_typeinf.CM_CC_THISCALL:
        print("[+] The function uses __thiscall.")
    else:
        return

    # Iterate through each argument
    for i in range(func_details.size()):
        arg: ida_typeinf.funcarg_t = func_details[i]       
        arg_type: ida_typeinf.tinfo_t = arg.type

        # Check if the argument is a pointer (common for struct arguments)
        is_ptr = arg_type.is_ptr()
        
        # Resolve the base type if it's a pointer/typedef
        base_type: ida_typeinf.tinfo_t = arg_type.get_pointed_object() if is_ptr else arg_type

        # Check if it is a UDT (User-Defined Type, i.e., Struct or Union)
        if base_type.is_udt():
            print("=> type_name:", base_type.get_type_name())
            return base_type
        else:
            print(f"Arg {i}: Not a structure type")
        
    return
    
if __name__ == '__main__':
    this_type = check_func_args_struct(idc.here())
    if this_type is not None:
        analyze_struct_offsets(idc.here(), "ecx", this_type)
