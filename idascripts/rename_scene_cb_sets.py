import idaapi
import idautils
import hxtb
import ida_kernwin
import ida_funcs
import idc

import ida_typeinf as tif
import ida_hexrays as hx



q_asgn_1 = lambda cf, i: (i.op is idaapi.cot_asg and
    i.x.op is idaapi.cot_memptr and
    i.x.x.op is idaapi.cot_var and
    i.y.op is idaapi.cot_obj)

q_asgn_2 = lambda cf, i: (i.op is idaapi.cot_asg and
    i.x.op is idaapi.cot_memptr and
    i.x.x.op is idaapi.cot_var and
    i.y.op is idaapi.cot_cast and
    i.y.x.op is idaapi.cot_obj)

func = ida_funcs.get_func(idc.here())

ea = func.start_ea
def has_namesace(fname):
    ns_split = fname.split("::")
    if len(ns_split) <= 1:
        return None
    return ns_split[0]

namespace = has_namesace(func.name)
if not namespace:
    raise Exception("missing namespace")

matches = []
def add_matches(i, sname = ''):
    global matches
    matches.append(i)

    return True

results = hxtb.find_item(ea, lambda cf, i: (q_asgn_1(cf, i) or q_asgn_2(cf, i)) and add_matches(i), parents=True)
# hxtb.ic_t(results)

print("Found ", len(results))

def get_type(i):
    base_type = i.x.x.type
    base_type.remove_ptr_or_array()
    return base_type

for i in matches:
    base_type = get_type(i)
    name = base_type.dstr()
    mem_offs = i.x.m
    if name != "CSceneCallbacks": continue
    
    udt_data = tif.udt_type_data_t()
    if not base_type.get_udt_details(udt_data): continue

    mem_name = None
    for udm in udt_data:
        mem_name = udm.name
        member_type = udm.type
        byte_offset = udm.offset // 8
        if byte_offset == mem_offs:
            mem_name = mem_name.removeprefix("pfn").partition("_")[0]
            break
    if not mem_name: continue
    
    if i.y.op == idaapi.cot_cast:
        fname = i.y.x.dstr()
    else:
        fname = i.y.dstr()

    fea = ida_name.get_name_ea(ida_idaapi.BADADDR, fname)
    if fea == ida_idaapi.BADADDR: continue
    
    ns = fname.split("::")[0]
    if ns == namespace: continue

    new_name = f"{namespace}::{mem_name}_{fea:06X}"
    new_decl = f"bool __cdecl {new_name}(CSBZGlobal *pGlobal);"
    
    print(new_name)
    ida_name.set_name(fea, new_name, ida_name.SN_FORCE)

    print(new_decl)
    idc.SetType(fea, new_decl)

    

    