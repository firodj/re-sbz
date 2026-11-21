import idautils
import idc
import ida_funcs
import ida_segment
import ida_typeinf

def scan_field(target_offset):
    # target_offset = 0x59C
    # target_name = "field_59C"
    
    print(f"[*] Scanning IDA 9.1 database for references to offset 0x{target_offset:X}...")

    found_structures = []

    # 1. Check local types/structures for members at offset 0x59C or named field_59C
    limit = ida_typeinf.get_ordinal_limit()
    print(f"[*] Try searching for {limit} structures")
    for ordinal in range(1, limit):
        tif = ida_typeinf.tinfo_t()
        if tif.get_numbered_type(None, ordinal) and tif.is_struct():
            sname = tif.get_type_name()
            udt_data = ida_typeinf.udt_type_data_t()
            if tif.get_udt_details(udt_data):
                for udm in udt_data:
                    mname = udm.name
                    moffset = udm.offset // 8  # convert bits to bytes
                    if (moffset == target_offset):
                        found_structures.append((sname, mname, moffset))
                    # if (target_name in mname):

    # Deduplicate structure findings
    found_structures = list(set(found_structures))
    if found_structures:
        print("\n[+] Found matching structure definitions in Local Types:")
        for sname, mname, moffset in found_structures:
            print(f"    Structure: {sname} | Member: {mname} | Offset: 0x{moffset:X}")
    
    print("Done.")
  

def scan_code(target_offset):
    
    print(f"\n[*] Scanning code segments for instructions referencing offset / immediate 0x{target_offset:X}...")
    
    total_refs = 0

    # 2. Scan code segments using segment objects correctly
    for seg_ea in idautils.Segments():
        seg = ida_segment.getseg(seg_ea)
        if not seg:
            continue
        seg_start = seg.start_ea
        seg_end = seg.end_ea
        
        for head in idautils.Heads(seg_start, seg_end):
            if idc.is_code(idc.get_full_flags(head)):
                for i in range(2):
                    if idc.get_operand_type(head, i) in [idc.o_imm, idc.o_displ, idc.o_phrase]:
                        if idc.get_operand_value(head, i) == target_offset:
                            total_refs += 1
                            func = ida_funcs.get_func(head)
                            fname = ida_funcs.get_func_name(head) if func else "unknown"
                            insn_text = idc.generate_disasm_line(head, 0)
                            print(f"    [0x{head:X}] ({fname}) -> {insn_text}")

    print(f"\n[*] Scan complete. Total code references found: {total_refs}")

if __name__ == "__main__":
    print("usage:")
    print("\tscan_code(<target_offset>)")
    print("\tscan_field(<offset>)")
