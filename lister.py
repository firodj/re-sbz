import idc
import idautils
import ida_segment


def get_segm_by_name(name):
    for seg_ea in idautils.Segments():
        seg_name = idc.get_segm_name(seg_ea)
        if name in seg_name:
            return ida_segment.getseg(seg_ea)
    return None

if __name__ == '__main__':
    segm = get_segm_by_name(".rdata")

    for head in idautils.Heads(segm.start_ea, segm.end_ea):
        name = idc.get_name(head)
        print(f"{hex(head)}: {name}")
