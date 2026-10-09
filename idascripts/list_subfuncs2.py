import idaapi
import idautils
import idc
import ida_xref
import ida_idp
import ida_ua

def get_callees(func_ea):
    """Return a list of function addresses called by the function at func_ea."""
    func = idaapi.get_func(func_ea)
    if not func:
        return []
    callees = set()
   
    # Iterate over all items in the function
    for head in idautils.FuncItems(func_ea):
        # Decode instruction
        insn = idaapi.insn_t()
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

    return list(callees)

def _is_func_ref(op):
    """Return True if operand references a function address (immediate value to func)."""
    # We consider immediate values that are function starts
    if op.type == idaapi.o_imm:
        if idaapi.get_func(op.value):
            return True
    # We could also consider memory operands that point to function addresses, but less common for args
    return False

def list_sub_functions(start_ea, visited=None, depth=0, path_last=[], is_last=True):
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