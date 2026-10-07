import ida_funcs
import ida_frame
import ida_typeinf

desired_size = 20
mark_name = 'var_3478'

valids = {
    50: [
        [(True, 8), (True, 392)],
        [(True, 32), (True, 352), (True, 16)],
        [(True, 376), (True, 16), (True, 8)],
        [(True, 64), (True, 320), (True, 16)],
    ],
    20: [
        [(True, 56), (True, 32), (True, 32), (True, 32), (True, 8)],
        [(True, 64), (True, 32), (True, 32), (True, 32)],
        [(True, 40), (True, 32), (True, 32), (True, 32), (True, 16), (True, 8)],
        [(True, 48), (True, 32), (True, 32), (True, 32), (True, 16)],
        [(True, 88), (True, 32), (True, 32), (True, 8)],
        [(True, 80), (True, 32), (True, 32), (True, 16)],
        [(True, 120), (True, 32), (True, 8)],
        [(True, 96), (True, 32), (True, 32)],
    ],
    25: [
        [(True, 112), (True, 32), (True, 32), (True, 16), (True, 8)],
        [(True, 104), (True, 32), (True, 32), (True, 32)],

        [(True, 120), (True, 32), (True, 32), (True, 16)],
        [(True, 96), (True, 32), (True, 32), (True, 32), (True, 8)],
        [(True, 88), (True, 32), (True, 32), (True, 32), (True, 16)],
        [(True, 136), (True, 32), (True, 32)],

        [(True, 48), (True, 32), (True, 32), (True, 32), (True, 32), (True, 16), (True, 8)],
    ]
}

addr_ = here()
pfn = ida_funcs.get_func(addr_)
tif = ida_typeinf.tinfo_t()

# Populate 'tif' with the function's structural frame definition
ida_frame.get_func_frame(tif, pfn)
    
udt_data = ida_typeinf.udt_type_data_t()
tif.get_udt_details(udt_data)

members = list(udt_data)

map_ofs = dict([(x.offset, i) for i,x in enumerate(members)])

mark_ofs = [x.offset for x in members if x.name == mark_name][0]

cnt = 0


while mark_ofs > 0:
    j = map_ofs[mark_ofs]
    prev_desired = mark_ofs - desired_size*8
    if prev_desired not in map_ofs:
        print("not found ofs", prev_desired)
        break
    
    i = map_ofs[prev_desired]
    member_name  = members[i].name
    offset_bytes = members[i].offset // 8
    
    byte_tif = ida_typeinf.tinfo_t(ida_typeinf.BTF_CHAR)  # standard 1-byte integer
    array_tif = ida_typeinf.tinfo_t()
    array_tif.create_array(byte_tif, desired_size)  # Convert into a 50-element array
  

    fact = []
    while i < j:
        is_var = members[i].name.startswith("var_")
        if len(fact) == 0 and not is_var: break


        delta_size = members[i+1].offset - members[i].offset
        fact.append( (is_var, delta_size))
        
        i += 1
    
    print(member_name , fact)
    if fact in valids[desired_size]:
        ida_frame.delete_frame_members(pfn, offset_bytes, offset_bytes + desired_size)

        success = ida_frame.add_frame_member(
            pfn,
            member_name ,
            offset_bytes,
            array_tif
        )
        
        if not success:
            print(f"    [!] Warning: Failed to apply member {member_name} at offset {offset_bytes}")

    # cnt += 1
    # if cnt > 5:
    #     break

    mark_ofs = prev_desired
  
    
    

