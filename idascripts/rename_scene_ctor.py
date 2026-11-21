import idaapi
import idautils
import hxtb
import ida_kernwin
import ida_funcs
import idc

import ida_typeinf as tif
import ida_hexrays as hx

matches = []
q_asgn = lambda cf, i: (i.op is idaapi.cot_asg and
    i.x.op is idaapi.cot_memptr and
    i.x.x.op is idaapi.cot_var)


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
def add_matches(i):
    global matches
    matches.append(i)
    return True

results = hxtb.find_item(ea, lambda cf, i: q_asgn(cf, i) and add_matches(i), parents=True)
# hxtb.ic_t(results)

print("Found ", len(results))

def get_type(x):
    base_type = x.type
    base_type.remove_ptr_or_array()
    return base_type

def get_member_name(typex, mem_offs):
    udt_data = tif.udt_type_data_t()
    if not typex.get_udt_details(udt_data):
        return False

    for idx, udm in enumerate(udt_data):
        mem_name = udm.name
        # member_type = udm.type
        byte_offset = udm.offset // 8
        if byte_offset == mem_offs:
            return idx, udm, mem_name

    return False

typemaps = dict(
    CSBZGlobal="pGlobal",
    CGameData="pGameData",
    CSBGameCore="pGameCore",
    CSceneRenderMgr="pSceneRenderMgr",
    CMouseState="pMouseState",
    CSB_CameraBase="pCameraBase",
    CSB_Key="pKey",
    CSB_Path="pPath",
    CSB_GameInfo="pGameInfo",
    CSB_Sound="pSound",
    CSB_CursorCtrl="pCursorCtrl"
)

for i in matches:
    base_type = get_type(i.x.x)
    typename = base_type.dstr()
    mem_offs = i.x.m
    idx, udm, mem_name = get_member_name(base_type, mem_offs)
    # typename == "SBResultScene"
    if not mem_name.startswith("field_"): continue
    
    if i.y.opname in ('var', 'memptr'):
        right_type = get_type(i.y)
    else:
        right_type = get_type(i.y.x)
    right_typename = right_type.dstr()

    print(typename, mem_name, idx, right_typename, i.y.dstr())

    if right_typename in typemaps:
        new_t = tif.tinfo_t()
        new_t.create_ptr(right_type)
        res = base_type.set_udm_type(idx, new_t)
        varname = typemaps[right_typename]
        if res != tif.TERR_OK:
            print("fail to retype")
            continue

        res = base_type.rename_udm(idx, varname)
        num = 1
        while res != tif.TERR_OK:
            num += 1
            if num > 3:
                print("fail to rename")
                break
            res = base_type.rename_udm(idx, f"{varname}{num}")


    