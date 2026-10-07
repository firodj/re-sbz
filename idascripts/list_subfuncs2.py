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

    return list(callees)

def list_sub_functions(start_ea, visited=None, depth=0):
    """Recursively traverse callees from start_ea, printing functions with 'sub_' prefix."""
    if visited is None:
        visited = set()
    if start_ea in visited:
        return 0
    visited.add(start_ea)
    func = idaapi.get_func(start_ea)
    if not func:
        return 0
    name = idaapi.get_func_name(start_ea)
    indent = "    " * depth
   
    if name.startswith("sub_"):
        print(f"{indent}0x{start_ea:x} {name}")
    # Recurse into callees
    n = 0
    callees = get_callees(start_ea)
    for callee in callees:
        n += list_sub_functions(callee, visited, depth + 1)
    return n+1

if __name__ == "__main__":
    # Start from the current screen address, or you can specify an address here.
    start_ea = idc.here()  # Current cursor address in IDA view
    print(f"Starting from 0x{start_ea:x}")
    n = list_sub_functions(start_ea)
    print(f"Done {n}")