import idc
import idautils
import ida_segment
import ida_bytes
import re

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

def align_up(ea, align):
    return (ea + align - 1) & ~(align - 1)

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

def maybe_shiftjis(data_bytes):
    return any((0xA1 <= x <= 0xDF) or (0x81 <= x <= 0x9F) for x in data_bytes)
    #customs = [x for x in data_bytes if (0xA1 <= x and x <= 0xDF)]
    #customs2 = [x for x in data_bytes if (0x81 <= x and x <= 0x9F)]
    #return len(customs) > 0 or len(customs2) > 0

def maybe_ascii(data_bytes):
    if data_bytes[-1] == 0:
        data_bytes = data_bytes[:-1]
    return all(0x20 <= x <= 0x7E or x in [0x0D, 0x0A, 0x09] for x in data_bytes)

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

def _yara_to_regex(hexstring):
    qmark = False
    for byte in hexstring.split():
        if byte == "??":
            out = ".."
            if not qmark:
                qmark = True
                out = "(" + out
        elif len(byte) == 2 and byte[0] == "?":
             
            out = ".)" + byte[1]
            if not qmark:
                out = "(" + out
                qmark = False
        elif len(byte) == 2 and byte[1] == "?":
            out = byte[0] + "(."
            qmark = True
        else:
            out = byte
            if qmark:
                out = ")" + out
                qmark = False
        
        yield out

    if qmark:
        yield ")"

def yara_to_regex(hexstring):
    return re.compile("".join(_yara_to_regex(hexstring)))

if __name__ == '__main__':
    segm = get_segm_by_name(".rdata")

    data = _yara_to_regex("55 8b ec 51 89 4d fc 8b e5 5d ( c3 | c2 ?? ?? )")
    print(data)
    rex = yara_to_regex("55 8b ec 51 89 4d fc 8b e5 5d ( c3 | c2 ?? ?? )")

