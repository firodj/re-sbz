# List all accesses to global variables in the current function
# Compatible with IDA Pro 9.1

import ida_segment
import idaapi
import idautils
import ida_funcs
import ida_name
import ida_lines
import ida_xref
import ida_ua

def is_operand_memory_write(insn, op):
    """
    Checks if a specific op_t operand is writing to a memory address.
    :param insn: The current decoded instruction (ida_ua.insn_t)
    :param op: The specific operand to evaluate (ida_ua.op_t)
    """
    # 1. Map the change flags to operand indices (0 to 5)
    OPND_WRITE_FLAGS = {
        0: idaapi.CF_CHG1,
        1: idaapi.CF_CHG2,
        2: idaapi.CF_CHG3,
        3: idaapi.CF_CHG4,
        4: idaapi.CF_CHG5,
        5: idaapi.CF_CHG6,
    }

    # 2. Verify the operand actually references memory
    is_mem = op.type in (ida_ua.o_mem, ida_ua.o_phrase, ida_ua.o_displ)
    if not is_mem:
        return False

    # 3. Pull the semantic feature tracking flags for this instruction
    feature = insn.get_canon_feature()
    
    # 4. Extract whether this specific operand index writes data
    write_flag = OPND_WRITE_FLAGS.get(op.n, 0)
    is_write = bool(feature & write_flag)

    return is_write


def is_write_operation(ea):
    """Determine if the instruction at ea is a write operation to memory using idaapi.decode_insn."""
    
    # Create an insn_t object to hold the decoded instruction
    insn = idaapi.insn_t()
    # Decode the instruction at ea
    if idaapi.decode_insn(insn, ea) == 0:
        # Failed to decode, assume read (safer)
        return False
    
    # Check each operand of the instruction
    for op in insn.ops:
        # If operand type is void, no more operands
        if op.type == idaapi.o_void:
            break
        
        # Check if operand is a memory reference
        # Memory reference types: o_mem, o_phrase, o_displ, o_far
        if is_operand_memory_write(insn, op):
            return True
    # No write memory operand found
    return False

def is_segment_writable(seg):
    """Check if the segment is writable."""
    if seg is None:
        return False
    # Check write permission: bit 2 (0x2) in seg.perm
    return bool(seg.perm & ida_segment.SEGPERM_WRITE)

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
                    access_type = "WRITE" if is_write_operation(ea) else "READ"
                    # Check if the segment is writable to determine if variable is const (read-only) or not
                    seg_writable = is_segment_writable(seg)
                    const_volatile = "READ-ONLY" if not seg_writable else "WRITABLE"
                    print(f"0x{ea:X}: {access_type} global variable 0x{ref_ea:X} ({name}) [{const_volatile}]")

    if count == 0:
        print("No accesses to global variables found in the current function.")

if __name__ == '__main__':
    list_global_accesses_in_func()