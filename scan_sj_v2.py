import idaapi
import idc
import idautils
import ida_nalt
import ida_bytes
import ida_segment

# pip install chardet
import chardet
print(chardet.__version__)

from .lister import get_segm_by_name

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
    for head in idautils.Heads(start_ea, end_ea):
        flags = ida_bytes.get_full_flags(head)
        # Focus on dummy names
        has_dummy_name = ida_bytes.has_dummy_name(flags)
        # Skip non-dummy names
        if not has_dummy_name:
            continue
        
        if ida_bytes.is_strlit(flags):
            content = ida_bytes.get_strlit_contents(head, -1, idc.STRTYPE_C)
            if len(content) > 1:
                continue

            if ida_nalt.get_str_encoding_idx(ida_nalt.get_str_type(head)) == sjis_idx:
                continue
                
        elif not ida_bytes.is_byte(flags):
            continue

        
        nullterminated = ida_bytes.find_byte(head, end_ea-head, 0, 0)
        if nullterminated < 0:
            continue
    
        sz = nullterminated - head
        if sz <= 0:
            continue

        data_bytes = ida_bytes.get_bytes(head, sz)
        
        try:
            det = chardet.detect(data_bytes)
        except Exception as e:
            print(f"An error occurred: {e}")
            continue
        
        if not det['encoding'] or det['encoding'].upper() != 'SHIFT_JIS':
            continue

        print(f"Processing {hex(head)}: {idc.get_name(head)}, size {sz}")
        ida_bytes.create_strlit(head, sz, new_type)
        
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



