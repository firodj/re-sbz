import idaapi
import idautils
import hxtb
import ida_kernwin

all_i = []
def adder(i, sname = ''):
    global all_i
    all_i.append(i)

    tif = i.type
    if not tif.is_udt():
        return False
    
    if tif.get_final_type_name() == sname:
        return True

    # print i.dstr()
    return False

q_only_struct = lambda cf, i: (i.op is idaapi.cot_memref and
    i.x.op is idaapi.cot_obj and adder(i, 'CSceneMenuState_98'))

ea = ida_funcs.get_func(idc.here()).start_ea

results = hxtb.find_item(ea, lambda cf, i: q_only_struct(cf, i), parents=True)
hxtb.ic_t(results)
