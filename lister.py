import idc
import idautils
import ida_segment
import ida_bytes

def get_segm_by_name(name):
    for seg_ea in idautils.Segments():
        seg_name = idc.get_segm_name(seg_ea)
        if name in seg_name:
            return ida_segment.getseg(seg_ea)
    return None

def list_everything(start_ea, end_ea):
    for ea in range(start_ea, end_ea):
        flags = ida_bytes.get_full_flags(ea)
        
        #has_xref = ida_bytes.has_xref(flags)
        has_any_name = ida_bytes.has_any_name(flags)

        if has_any_name:
            name = idc.get_name(ea)
            yield ea, name

def list_all_Heads(start_ea, end_ea):
    for head in idautils.Heads(start_ea, end_ea):
        name = idc.get_name(head)
        print(f"{hex(head)}: {name}")

if __name__ == '__main__':
    segm = get_segm_by_name(".rdata")

    for ea, name in list_everything(segm.start_ea, 0x7ee8f0):
        print(f"{hex(ea)}: {name}")
