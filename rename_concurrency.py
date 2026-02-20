import idautils
import ida_funcs
import ida_ua
import ida_name
import idc
import ida_bytes

def get_getter_offset(func_ea):
    # Pattern we are looking for:
    # 0: push ebp
    # 1: mov ebp, esp
    # 2: push ecx
    # 3: mov [ebp+var_4], ecx
    # 4: mov eax, [ebp+var_4]
    # 5: mov eax, [eax + OFFSET]  <-- The key instruction
    # 6: mov esp, ebp
    # 7: pop ebp
    # 8: retn
    
    # Check instruction count (rough check)
    func = ida_funcs.get_func(func_ea)
    if not func: return None
    
    # We expect exactly 9 instructions for a simple getter
    # Iterate instructions
    insns = []
    curr = func.start_ea
    while curr < func.end_ea:
        insns.append(curr)
        curr = idc.next_head(curr)
    
    if len(insns) != 9:
        return None

    # Check key instruction at index 5
    key_ea = insns[5]
    mnem = idc.print_insn_mnem(key_ea)
    
    if mnem != "mov":
        return None
        
    # Check operands: op1=eax, op2=[eax+OFFSET]
    # We can check op types. 
    # op1 type should be o_reg (1), reg should be 0 (eax)
    # op2 type should be o_displ (4) or o_phrase (3 - for [eax])
    
    insn = ida_ua.insn_t()
    ida_ua.decode_insn(insn, key_ea)
    
    if insn.Op1.type != ida_ua.o_reg or insn.Op1.reg != 0: # eax
        return None
        
    offset = 0
    if insn.Op2.type == ida_ua.o_phrase:
        # [eax] -> offset 0
        if insn.Op2.phrase != 0: # phrase 0 is usually [eax] or [ax] depending, simple check
             # Double check text representation just to be safe
             disasm = idc.generate_disasm_line(key_ea, 0)
             if "[eax]" not in disasm:
                 return None
        offset = 0
        
    elif insn.Op2.type == ida_ua.o_displ:
        # [eax+OFFSET]
        offset = insn.Op2.addr
    else:
        return None

    # Verify context instructions to ensure it's actually 'this' pointer usage
    # instruction 4: mov eax, [ebp+var_4]
    prev_ea = insns[4]
    insn_prev = ida_ua.insn_t()
    ida_ua.decode_insn(insn_prev, prev_ea)
    if insn_prev.itype != ida_ua.NN_mov: return None
    if insn_prev.Op1.type != ida_ua.o_reg or insn_prev.Op1.reg != 0: return None # eax
    
    return offset

def rename_concurrency_getters():
    print("Scanning for @Concurrency@ getter functions...")
    count = 0
    renamed = 0
    
    for func_ea in idautils.Functions():
        name = idc.get_name(func_ea)
        
        if "@Concurrency@" in name and "Get_Member_" not in name:
            offset = get_getter_offset(func_ea)
            
            if offset is not None:
                new_name = f"Get_Member_{offset:X}_Val_{func_ea:X}"
                
                print(f"Match at {hex(func_ea)}: Offset {hex(offset)}")
                print(f"  Old: {name}")
                print(f"  New: {new_name}")
                
                if ida_name.set_name(func_ea, new_name, ida_name.SN_NOWARN):
                    renamed += 1
                else:
                    print(f"  Failed to rename {hex(func_ea)}")
                
                count += 1
                
    print(f"Scan complete. Found {count} getters. Renamed {renamed}.")

if __name__ == "__main__":
    rename_concurrency_getters()
