import idaapi
import idc
import idautils
import ida_nalt
import ida_bytes
import ida_segment
import importlib

# pip install chardet
#import chardet
#print(chardet.__version__)

import lister
importlib.reload(lister)

from lister import get_segm_by_name, list_everything


def get_or_create_encoding(name):
    # Try using add_encoding logic or just listing
    # We don't want to add multiple Shift-JIS
    enc_idx = -1
    qty = ida_nalt.get_encoding_qty()
    for i in range(qty):
        enc_name = ida_nalt.get_encoding_name(i)
        if enc_name and name.lower() == enc_name.lower():
            print(f"Found encoding '{enc_name}' at index {i}")
            return i

    # If not found, create
    print(f"Creating encoding '{name}'...")
    enc_idx = ida_nalt.add_encoding(name)
    if enc_idx >= 0:
        print(f"Created encoding '{name}' at index {enc_idx}")
    else:
        print(f"Failed to create encoding '{name}'")
    return enc_idx

def scan_and_set_encoding():
    sjis_idx = get_or_create_encoding("Shift-JIS")
    if sjis_idx < 0:
        print("Could not get Shift-JIS encoding index.")
        return

    # Find .rdata
    start_ea = None
    end_ea = None
    
    seg = get_segm_by_name(".rdata")
    if seg is None:
        print("No .rdata segment found.")
        return
    start_ea = seg.start_ea
    end_ea = seg.end_ea
    #end_ea = 0x007ED8F0 + 0x1000
    print(f"Found .rdata: ({hex(start_ea)} - {hex(end_ea)})")
    
    # Using STRTYPE_C (0) + Shift-JIS
    new_type = ida_nalt.make_str_type(idc.STRTYPE_C, sjis_idx)

    count = 0
    stop = 0
  
    # name in idautils.Names():
    #    if head < start_ea: continue
    #    if head >= end_ea: continue
    #    print(f"Processing {hex(head)}: {name}")

    # Iterate heads
    for ea, name in list_everything(start_ea, end_ea):
        flags = ida_bytes.get_full_flags(ea)
        # Focus on dummy names
        has_dummy_name = ida_bytes.has_dummy_name(flags)
        # Skip non-dummy names
        if not has_dummy_name:
            if not ida_bytes.is_strlit(flags): continue
            content = ida_bytes.get_strlit_contents(ea, -1, idc.STRTYPE_C)
            if len(content) <= 1: continue
            ea_next = ida_bytes.get_item_end(ea)
            if ea_next <= ea or ea_next == idc.BADADDR: continue

            flags_next = ida_bytes.get_full_flags(ea_next)
            if not ida_bytes.is_unknown(flags_next) or ida_bytes.get_byte(ea_next-1) == 0:
                continue
        elif ida_bytes.is_strlit(flags):
            content = ida_bytes.get_strlit_contents(ea, -1, idc.STRTYPE_C)
            if len(content) > 1:
                continue

            if ida_nalt.get_str_encoding_idx(ida_nalt.get_str_type(ea)) == sjis_idx:
                continue
                
        elif not ida_bytes.is_byte(flags) and not ida_bytes.is_unknown(flags):
            continue

        nullterminated = idc.BADADDR
        for find_ea in range(ea, end_ea):
            if ida_bytes.get_byte(find_ea) == 0:
                nullterminated = find_ea+1
                break
            if find_ea != ea:
                find_flags = ida_bytes.get_full_flags(find_ea)
                if ida_bytes.has_any_name(find_flags):
                    break
        #nullterminated = ida_bytes.find_byte(ea, end_ea-ea, 0, 0)
        if nullterminated == idc.BADADDR:
            continue
    
        sz = nullterminated - ea
        if sz <= 0:
            continue

        data_bytes = ida_bytes.get_bytes(ea, sz, ida_bytes.GMB_READALL)
        customs = [x for x in data_bytes if (0xA1 <= x and x <= 0xDF)]
        customs2 = [x for x in data_bytes if (0x81 <= x and x <= 0x9F)]
        if len(customs) == 0 and len(customs2) == 0:
            continue

        print(f"Processing {hex(ea)}: {name}, size {sz}")
        ida_bytes.create_strlit(ea, sz, new_type)
        
        count += 1

        if stop:
            stop -= 1
            if stop <= 0:
                break
        

    print(f"Processed {count} strings in .rdata.")
    idaapi.request_refresh(idaapi.IWID_DISASMS)
    idaapi.request_refresh(idaapi.IWID_STRINGS)

if __name__ == "__main__":
    scan_and_set_encoding()



