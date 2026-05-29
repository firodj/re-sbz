import idaapi
import idautils
import hxtb
import ida_kernwin


q_only_op_new = lambda cf, i: (i.op is idaapi.cot_call and
    i.x.op is idaapi.cot_obj and
    idaapi.get_name( i.x.obj_ea) == '??2@YAPAXI@Z' and
    i.a[0].op is idaapi.cot_num)

q_call_new = lambda cf, i: (i.op is idaapi.cit_expr and
    i.cexpr.op is idaapi.cot_asg and
    i.cexpr.x.op is idaapi.cot_var and
    i.cexpr.y.op is idaapi.cot_call and
    i.cexpr.y.x.op is idaapi.cot_obj and
    idaapi.get_name(i.cexpr.y.x.obj_ea) == '??2@YAPAXI@Z' and
    i.cexpr.y.a[0].op is idaapi.cot_num)

q_call_new_with_cast =  lambda cf, i:  (i.op is idaapi.cit_expr and
    i.cexpr.op is idaapi.cot_asg and
    i.cexpr.x.op is idaapi.cot_var and
    i.cexpr.y.op is idaapi.cot_cast and
    i.cexpr.y.x.op is idaapi.cot_call and
    i.cexpr.y.x.x.op is idaapi.cot_obj and
    idaapi.get_name( i.cexpr.y.x.x.obj_ea) == '??2@YAPAXI@Z' and
    i.cexpr.y.x.a[0].op is idaapi.cot_num)

q_call_if_within_new =  lambda cf, i: (i.op is idaapi.cit_if and 
    i.cif.expr.op is idaapi.cot_call and
    i.cif.expr.x.op is idaapi.cot_obj and
    idaapi.get_name(i.cif.expr.x.obj_ea) == '??2@YAPAXI@Z' and 
    i.cif.expr.a[0].op is idaapi.cot_num)

q_if_block = lambda cf, i: (i.op is idaapi.cit_if and
    i.cif.expr.op is idaapi.cot_var and
    i.cif.ithen.op is idaapi.cit_block and
    i.cif.ithen.cblock[0].op is idaapi.cit_expr and
    i.cif.ithen.cblock[0].cexpr.op is idaapi.cot_asg and
    i.cif.ithen.cblock[0].cexpr.x.op is idaapi.cot_var and
    i.cif.ithen.cblock[0].cexpr.y.op is idaapi.cot_call and
    i.cif.ithen.cblock[0].cexpr.y.x.op is idaapi.cot_obj and
    i.cif.ielse.op is idaapi.cit_block and
    i.cif.ielse.cblock[0].op is idaapi.cit_expr and
    i.cif.ielse.cblock[0].cexpr.op is idaapi.cot_asg and
    i.cif.ielse.cblock[0].cexpr.x.op is idaapi.cot_var)

founds = []
def add_founds(cf, i, mode):
    limit_cap = 3

    # block = new(block_size)
    block_ptr = i.cexpr.x.v
    block_ptr_idx = i.cexpr.x.v.idx
    block_ptr_name = i.cexpr.x.v.getv().name

    if mode == 1:
        block_size = i.cexpr.y.a[0].numval()
    elif mode == 2:
        block_size = i.cexpr.y.x.a[0].numval()

    result = {
        "ea": i.ea,
        "dstr": i.dstr(),
        "block_ptr_idx": block_ptr_idx,
        "block_ptr_name": block_ptr_name,
        "block_size": block_size,
    }

    print("block:", result)
  
    parent = cf.body.find_parent_of(i)
    next_siblings = []
    if_stmt = None
    
    if parent and parent.op == idaapi.cit_block:
        cap = False
        cap_count = 0

        for st in parent.cinsn.cblock:
           
            if cap:
                cap_count += 1
                if cap_count > limit_cap:
                    return False

                if q_if_block(cf, st):
                    #print(st.dstr())

                    if_block_ptr = st.cif.expr.v
                    if_block_ptr_idx = st.cif.expr.v.idx
                    # if (Block) vvv = Construtor(...) else vvv = ...
                    #print("->",if_block_ptr_idx,block_ptr_idx )
                    if if_block_ptr_idx != block_ptr_idx:
                        continue

                    ifthen_v = st.cif.ithen.cblock[0].cexpr.x.v.idx
                    ifelse_v = st.cif.ielse.cblock[0].cexpr.x.v.idx
                    
                    #print("-->", ifthen_v, ifelse_v)
                    if ifthen_v != ifelse_v:
                        continue
                    
                    instance_name = st.cif.ithen.cblock[0].cexpr.x.v.getv().name
                    constructor_ea = st.cif.ithen.cblock[0].cexpr.y.x.obj_ea
                    constructor_name = idaapi.get_name(constructor_ea)
                    result["constructor_name"] = constructor_name
                    result["constructor_ea"] = constructor_ea
                    result["instnace_name"] = instance_name
                    result["instnace_idx"] = ifthen_v
                    result["dstr"] += st.dstr()

                    if_stmt = st
                    break
                
            elif st == i:
                cap = True

        if not if_stmt:
            return False
    else:
        return False
    
    if not if_stmt:
        return False

    founds.append(result)
    return True

def redec_founds(cf, i):
    then_st = lambda cf, i: (i.op is idaapi.cit_if and
        i.cif.ithen.op is idaapi.cit_block and
        i.cif.ithen.cblock[0].op is idaapi.cit_expr and
        i.cif.ithen.cblock[0].cexpr.op is idaapi.cot_asg and
        i.cif.ithen.cblock[0].cexpr.x.op is idaapi.cot_var and
        i.cif.ithen.cblock[0].cexpr.y.op is idaapi.cot_call and
        i.cif.ithen.cblock[0].cexpr.y.x.op is idaapi.cot_obj)
  
    if not then_st(cf, i):
        return False

    constructor_ea = i.cif.ithen.cblock[0].cexpr.y.x.obj_ea
    constructor_name = idaapi.get_name(constructor_ea)
    
    founds.append({
        'constructor_name': constructor_name,
        'constructor_ea': constructor_ea,
        'cf_entry_ea': cf.entry_ea
    })

    print(f"Force decompile constructor {hex(constructor_ea)} {constructor_name} ...")
    constructor_cfunc = ida_hexrays.decompile(constructor_ea)

    print(f"Force decompile current func {hex(cf.entry_ea)} ...")
    ida_hexrays.decompile(cf.entry_ea)

    return True


class display_result_t(ida_kernwin.Choose):
    window_title = "SBZ Result"

    def __init__(self, founds):
        self.founds = founds

        _title = self._find_win_title()

        ida_kernwin.Choose.__init__(
            self,
            _title,
            [ ["Constructor", 20 | ida_kernwin.CHCOL_FNAME],
              ["Address", 10 | ida_kernwin.CHCOL_EA],
              ["BlockSize", 10 | ida_kernwin.CHCOL_HEX],
              ["Output", 80 | ida_kernwin.CHCOL_PLAIN]
              ],
            #flags = flags,
            #width = width,
            #height = height,
            #embedded = embedded
            )

        self.Show()
    
    def _find_win_title(self):
        i = 0
        idx = ""
        pfx = ""
        exists = True
        while exists:
            idx = chr(ord('A')+i%26)
            _title = "%s-%s%s" % (display_result_t.window_title, pfx, idx)
            #if title:
            #    _title += ": %s" % title
            exists = (ida_kernwin.find_widget(_title) != None)
            i += 1
            pfx += "" if i % 26 else "A"

        return _title

    def OnGetSize(self):
        # Return the number of items in the list
        return len(self.founds)

    def OnGetLine(self, n):
        # Return a list of strings representing the columns for the n-th item
        return [self.founds[n]["constructor_name"], 
            hex(self.founds[n]["constructor_ea"]), 
            hex(self.founds[n]["block_size"]),
            self.founds[n]["dstr"]
            ]
    
    def OnClose(self):
        self.founds = []

    def OnSelectLine(self, n):
        item_ea = self.founds[n]["ea"]
        func_ea = self.founds[n]["constructor_ea"]
        ea = func_ea if item_ea == BADADDR else item_ea
        ida_kernwin.jumpto(ea)

CASE = 1

#eas = [here()]
eas = list(idautils.Functions())

for ix, ea in enumerate(eas):
    print(f"function {ix+1}/{len(eas)} at {hex(ea)} ...")
    if CASE == 1:
        the_news = hxtb.find_item(ea, lambda cf, i:
            (q_call_new(cf,i) and add_founds(cf, i, 1)) or
            (q_call_new_with_cast(cf, i) and add_founds(cf, i, 2)), parents=True)

        #print(the_news)

    if CASE == 2:
        the_news = hxtb.find_item(ea, lambda cf, i:
            (q_call_if_within_new(cf,i) and redec_founds(cf, i)), parents=True)

        #print(the_news)
        #print(founds)

display_result = display_result_t(founds)
