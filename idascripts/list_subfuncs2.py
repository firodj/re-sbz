import idaapi
import idautils
import idc
import ida_xref
import ida_idp
import ida_ua
import ida_bytes
import ida_ida

skip_func = set([
    "??2@YAPAXI@Z", # new
    "??3@YAXPAX@Z", # free
])

def get_callees(func_ea):
    """Return a list of function addresses called by the function at func_ea."""
    func = idaapi.get_func(func_ea)
    if not func:
        return []
    callees = set()
    seen_vtables = set()
   
    # Iterate over all items in the function
    for head in idautils.FuncItems(func_ea):
        # Decode instruction
            
        if idaapi.decode_insn(insn, head) == 0:
            continue
        if ida_idp.is_call_insn(insn):
            # Get the first code reference from this instruction
            target = idc.get_operand_value(head, 0)
            
            if target != idc.BADADDR:
                callee = idaapi.get_func(target)
                if callee:
                    callees.add(callee.start_ea)
        else:
            for op in insn.ops:
                if _is_func_ref(op):
                    target = op.value
                    if target != idc.BADADDR:
                        callee = idaapi.get_func(target)
                        if callee:
                            callees.add(callee.start_ea)
                if op.type == idaapi.o_imm:
                    addr = op.value
                    if addr in seen_vtables:
                        continue
                    pointer_size = 4 if ida_ida.idainfo_is_32bit() else 8
                    max_count = 20
                    if is_vtable(addr, pointer_size, max_count):
                        seen_vtables.add(addr)
                        print(f"Possible vtable {addr}")
                        vcallees = print_vtable(addr, pointer_size, max_count)
                        callees.update(vcallees)
    return list(callees)


def is_vtable(addr, pointer_size, max_count=10):
    """Check if the given address looks like a VTable."""
    if addr == BADADDR:
        return False
    seg = ida_segment.getseg(addr)
    if seg is None:
        return False
    # Must be in a readable, non-executable segment (data section)
    if not (seg.perm & ida_segment.SEGPERM_READ) or (seg.perm & ida_segment.SEGPERM_EXEC):
        return False
    
    count = 0
    valid_count = 0
    for i in range(max_count):
        ptr_addr = addr + i * pointer_size
        if ptr_addr == BADADDR:
            break
        # Read pointer value
        if pointer_size == 4:
            ptr = ida_bytes.get_dword(ptr_addr)
        else:
            ptr = ida_bytes.get_qword(ptr_addr)
        if ptr == BADADDR:
            break
        if is_code_ptr(ptr):
            valid_count += 1
        count += 1
        # Early exit if we have enough valid pointers
        if valid_count >= max_count // 2 and i >= max_count // 2:
            return True
    return valid_count >= count // 2 if count > 0 else False

def is_code_ptr(ptr):
    """Check if a pointer points to executable memory."""
    if ptr == 0 or ptr == BADADDR:
        return False
    seg = ida_segment.getseg(ptr)
    if seg is None:
        return False
    return bool(seg.perm & ida_segment.SEGPERM_EXEC)

def print_vtable(vtable_addr, pointer_size, max_count=10):
    """Print details of the found VTable."""
    callees = set()
    # print("  VTable contents:")
    for i in range(max_count):
        ptr_addr = vtable_addr + i * pointer_size
        if ptr_addr == BADADDR:
            break
        if pointer_size == 4:
            ptr = ida_bytes.get_dword(ptr_addr)
        else:
            ptr = ida_bytes.get_qword(ptr_addr)
        if ptr == BADADDR or ptr == 0:
            break
        callee = idaapi.get_func(ptr)
        if callee:
            callees.add(ptr)
        #     func_name = idc.get_name(ptr, idaapi.GN_VISIBLE)
        #     if func_name:
        #         print("  [%d] 0x%x (%s)" % (i, ptr, func_name))
        #     else:
        #         print("  [%d] 0x%x" % (i, ptr))
        # else:
        #     break
    return callees

def _is_func_ref(op):
    """Return True if operand references a function address (immediate value to func)."""
    # We consider immediate values that are function starts
    if op.type == idaapi.o_imm:
        if idaapi.get_func(op.value):
            return True
    # We could also consider memory operands that point to function addresses, but less common for args
    return False

def list_sub_functions(start_ea, visited=None, depth=0, path_last=[], is_last=True):
    global skip_func

    
    """Recursively traverse callees from start_ea, returning functions with 'sub_' prefix as lines."""
    if visited is None:
        visited = set()
    if start_ea in visited:
        return []
    visited.add(start_ea)
    func = idaapi.get_func(start_ea)
    if not func:
        return []
    name = idaapi.get_func_name(start_ea)
    
    if name in skip_func:
        return []
    
    lines = []
    # If the function name starts with "sub_", we add it to our lines
    # ok = name.startswith("sub_")
    ok = True
    if ok:
        if depth == 0:
            line = f"0x{start_ea:x} {name}"
        else:
            # Build the prefix for the levels 0 to depth-1
            prefix = ""
            for i in range(depth):
                if i < len(path_last):
                    if path_last[i]:
                        prefix += "    "
                    else:
                        prefix += "│   "
                else:
                    # This should not happen if we are consistent
                    prefix += "    "
            # Now add the connector for the current depth
            if is_last:
                prefix += "└──"
            else:
                prefix += "├──"
            line = prefix + f"0x{start_ea:x} {name}"
        lines.append(line)
    
    # Recurse into callees
    callees = get_callees(start_ea)
    for i, callee in enumerate(callees):
        # Determine if this callee is the last child
        child_is_last = (i == len(callees) - 1)
        lines.extend(list_sub_functions(callee, visited, depth+1, path_last + [is_last], child_is_last))
    return lines

if __name__ == "__main__":
    # Start from the current screen address, or you can specify an address here.
    start_ea = idc.here()  # Current cursor address in IDA view
    print(f"Starting from 0x{start_ea:x}")
    lines = list_sub_functions(start_ea)
    for line in lines:
        print(line)
    print(f"Done {len(lines)}")