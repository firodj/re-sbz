import idaapi
import idc
import idautils
import ida_segment
import ida_bytes
import ida_kernwin

idaapi.require('miscs')
from miscs import *

def get_segm_by_name(name):
    for seg_ea in idautils.Segments():
        seg_name = idc.get_segm_name(seg_ea)
        if name in seg_name:
            return ida_segment.getseg(seg_ea)
    return None

def list_everything(start_ea, end_ea):
    ea = start_ea
    while ea < end_ea:
        flags = ida_bytes.get_full_flags(ea)
        
        #has_xref = ida_bytes.has_xref(flags)
        has_any_name = ida_bytes.has_any_name(flags)

        if has_any_name:
            name = idc.get_name(ea)
            yield ea, name
    
        next_ea = ida_bytes.get_item_end(ea)
        if next_ea == ea:
            next_ea = ea + 1
        ea = next_ea

def list_unknowns(start_ea, end_ea, pred = None):
    print(f"list unknowns ... ({hex(start_ea)} - {hex(end_ea)})")
    ea = align_up(start_ea, 4)
    while ea < end_ea:
        flags = ida_bytes.get_full_flags(ea)
        if ida_bytes.is_unknown(flags):
            if pred is None or pred(ea, flags):
                yield ea
                
        next_ea = ida_bytes.next_unknown(ea, end_ea)
        if next_ea == ea:
            next_ea = ea + 1
        ea = align_up(next_ea, 4)

def list_all_Heads(start_ea, end_ea):
    for head in idautils.Heads(start_ea, end_ea):
        name = idc.get_name(head)
        print(f"{hex(head)}: {name}")

def find_nullterminated(ea, end_ea=None):
    if end_ea is None:
        end_ea = ida_segment.getseg(ea).end_ea
    nullterminated = idc.BADADDR
    for find_ea in range(ea, end_ea):
        if ida_bytes.get_byte(find_ea) == 0:
            nullterminated = find_ea+1
            break
        if find_ea != ea:
            find_flags = ida_bytes.get_full_flags(find_ea)
            if ida_bytes.has_any_name(find_flags):
                break
    return nullterminated


def item_end_is_notnull(ea):
    ea_next = ida_bytes.get_item_end(ea)
    if ea_next <= ea or ea_next == idc.BADADDR: return False
    flags_next = ida_bytes.get_full_flags(ea_next)
    return ida_bytes.is_unknown(flags_next) and ida_bytes.get_byte(ea_next-1) != 0

def explain_flags(ea):
    flags =ida_bytes.get_full_flags(ea)
    result = []
    if ida_bytes.is_code(flags):
        result.append("code")
    if ida_bytes.is_data(flags):
        result.append("data")
    if ida_bytes.is_unknown(flags):
        result.append("unknown")
    if ida_bytes.is_head(flags):
        result.append("head")
    if ida_bytes.is_tail(flags):
        result.append("tail")
    if ida_bytes.is_flow(flags):
        result.append("flow")
    if ida_bytes.has_cmt(flags):
        result.append("cmt")
    if ida_bytes.has_extra_cmts(flags):
        result.append("extra_cmts")
    if ida_bytes.has_xref(flags):
        result.append("xref")
    if ida_bytes.has_any_name(flags):
        result.append("any_name")
    if ida_bytes.has_dummy_name(flags):
        result.append("dummy_name")
    if ida_bytes.has_auto_name(flags):
        result.append("auto_name")
    if ida_bytes.has_user_name(flags):
        result.append("user_name")
    if ida_bytes.has_name(flags):
        result.append("name")
    if ida_bytes.is_byte(flags):
        result.append("byte")
    if ida_bytes.is_word(flags):
        result.append("word")
    if ida_bytes.is_dword(flags):
        result.append("dword")
    if ida_bytes.is_qword(flags):
        result.append("qword")
    if ida_bytes.is_strlit(flags):
        result.append("strlit")
        #print(ida_nalt.get_str_type(idc.get_screen_ea()))
    if ida_bytes.is_struct(flags):
        result.append("struct")
    if ida_bytes.is_enum0(flags):
        result.append("enum0")
    if ida_bytes.is_enum1(flags):
        result.append("enum1")

    # get current segemnt
    seg = ida_segment.getseg(ea)

    return {
        "flags": result,
        
        "item_size": ida_bytes.get_item_size(ea),
        "item_head": hex(ida_bytes.get_item_head(ea)),
        "item_end": hex(ida_bytes.get_item_end(ea)),
        "next_head": hex(ida_bytes.next_head(ea, seg.end_ea)),
        "next_unknown": hex(ida_bytes.next_unknown(ea, seg.end_ea)),
        "prev_head": hex(ida_bytes.prev_head(ea, seg.start_ea)),
        "prev_unknown": hex(ida_bytes.prev_unknown(ea, seg.start_ea)),
    }



def get_selected_bytes():
    view = ida_kernwin.get_current_viewer()
    selection = ida_kernwin.read_range_selection(view)

    if selection is None or not selection[0]:
        return None
    return ida_bytes.get_bytes(selection[1], selection[2] - selection[1])

if __name__ == '__main__':
    segm = get_segm_by_name(".rdata")

    print(get_selected_bytes().hex(' '))
