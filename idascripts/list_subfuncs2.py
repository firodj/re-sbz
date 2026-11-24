import idaapi
import idautils
import idc
import ida_xref
import ida_idp
import ida_ua
import ida_bytes
import ida_ida
import ida_segment
import ida_nalt

skip_func = set([
    "??2@YAPAXI@Z", # new
    "j_??2@YAPAXI@Z", # new
    "??3@YAXPAX@Z", # free
])

def get_callees(func_ea):
    """Return a list of function addresses called by the function at func_ea."""
    func = idaapi.get_func(func_ea)
    if not func:
        return []
    callees = set()
    seen_vtables = set()
   
    optEnableVTables = False

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
                has_name = idaapi.get_name(target)
                if callee:
                    callees.add(callee.start_ea)
                elif has_name:  
                    if has_name:
                        callees.add(target)
                else:

                    for xref in idautils.XrefsFrom(head, ida_xref.XREF_FAR):
                        target = xref.to
                       
                        callee = idaapi.get_func(target)
                        if callee:
                            callees.add(target)
        else:
            for op in insn.ops:
                if _is_func_ref(op):
                    target = op.value
                    if target != idc.BADADDR:
                        callee = idaapi.get_func(target)
                        if callee:
                            callees.add(callee.start_ea)

                if optEnableVTables:
                    if op.type == idaapi.o_imm:
                        addr = op.value
                        if addr in seen_vtables:
                            continue
                        pointer_size = 4 if ida_ida.idainfo_is_32bit() else 8
                        max_count = 20
                        if is_vtable(addr, pointer_size, max_count):
                            seen_vtables.add(addr)
                            
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

def _is_func_indirect(op):
    indirect_types = (
        ida_ua.o_reg,     # call eax
        ida_ua.o_phrase,  # call [edx] / call [edx+eax]
        ida_ua.o_displ,   # call [edx+10h]
    )
    return op.type in indirect_types

class AddLineParam:
    def __init__(self):
        self.depth =0
        self.path_last=[]
        self.is_last=True

    def advance(self, is_last):
        new_param = AddLineParam()
        new_param.depth = self.depth+1
        new_param.path_last = self.path_last + [self.is_last]
        new_param.is_last = is_last
        return new_param

def add_line(param: AddLineParam, target:int, name:str) -> str:
    if param.depth == 0:
        line = f"0x{target:x} {name}"
    else:
        # Build the prefix for the levels 0 to depth-1
        prefix = ""
        for i in range(param.depth):
            if i < len(param.path_last):
                if param.path_last[i]:
                    prefix += "    "
                else:
                    prefix += "│   "
            else:
                # This should not happen if we are consistent
                prefix += "    "
        # Now add the connector for the current depth
        if param.is_last:
            prefix += "└──"
        else:
            prefix += "├──"
        line = prefix + f"0x{target:x} {name}"
    print(f"{line}")
    return line

def is_import_stub(ea):
    seg = ida_segment.getseg(ea)
    if not seg:
        return False
    
    # Check if segment type is 'X' (external) or if segment name contains idata
    seg_name = ida_segment.get_segm_name(seg)
    return seg.type == ida_segment.SEG_XTRN or "idata" in seg_name.lower()

addr_to_mod_names = dict()

def get_import_module_by_ea(target_ea: int) -> str | None:
    global addr_to_mod_names
    if target_ea in addr_to_mod_names:
        return addr_to_mod_names[target_ea]
    
    qty = ida_nalt.get_import_module_qty()

    for i in range(qty):
        mod_name = ida_nalt.get_import_module_name(i)
        if not mod_name:
            continue

        found = False

        def imp_cb(ea, name, ordinal):
            nonlocal found
            if ea == target_ea:
                found = True
                return False  # Stop enumerating this module
            addr_to_mod_names[ea] = mod_name
            return True  # Continue

        ida_nalt.enum_import_names(i, imp_cb)
        if found:
            return mod_name

    return None


def list_sub_functions(start_ea, visited=None, param = AddLineParam()):
    global skip_func

    """Recursively traverse callees from start_ea, returning functions with 'sub_' prefix as lines."""
    if visited is None:
        visited = set()
    if start_ea in visited:
        return []
    visited.add(start_ea)
    func = idaapi.get_func(start_ea)

    if not func:
        name = idaapi.get_name(start_ea)
        if name:
            if (is_import_stub(start_ea)):
                mod_name = get_import_module_by_ea(start_ea)
                if mod_name:
                    name = f"{mod_name}!{name}"

            return [ add_line(param, start_ea, name)]
    name = idaapi.get_func_name(start_ea)
    
    if name.startswith("_") and name not in ('_WinMain@16'): return []
    if name in skip_func: return []
    if name.startswith("D3DX"): return []
    if name.startswith("CString"): return []
    if name.startswith("CVector"): return []
    if name.startswith("InitScene"): return []
    
    lines = []

    # If the function name starts with "sub_", we add it to our lines
    # ok = name.startswith("sub_")
    line = add_line(param, start_ea, name)
    lines.append(line)
    
    # Recurse into callees
    callees = get_callees(start_ea)
    for i, callee in enumerate(callees):
        # Determine if this callee is the last child
        child_is_last = (i == len(callees) - 1)
        sub_lines = list_sub_functions(callee, visited, param.advance(child_is_last))
        # lines.extend(sub_lines)
    return lines

if __name__ == "__main__":
    # Start from the current screen address, or you can specify an address here.
    start_ea = idc.here()  # Current cursor address in IDA view
    func = idaapi.get_func(start_ea)

    if func:
        print(f"Starting from 0x{func.start_ea:x}")
        lines = list_sub_functions(func.start_ea)
        # for line in lines:
        #     print(line)
        print(f"Done {len(lines)}")