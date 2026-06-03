import idaapi
import idautils
import hxtb
import ida_kernwin

q_memset = lambda cf, i: (i.op is idaapi.cit_expr and
    i.cexpr.op is idaapi.cot_call and
    i.cexpr.x.op is idaapi.cot_obj and
    idaapi.get_name(i.cexpr.x.obj_ea) in ('memset', '_memset'))

def get_computed_sizeof(cf, i):
    """
    Takes a cexpr_t of op type cot_sizeof and returns its computed integer size.
    """
    print(i)
    expr = i.cexpr.a[2]

    if expr.op == idaapi.cot_num:
        sz = expr.numval()
        return sz

    if expr.op != idaapi.cot_sizeof:
        print(f"[-] [{i.ea:X}] Provided expression is not a cot_sizeof operator.")
        return None

    # The operand 'x' is the expression or type inside the sizeof(...)
    target_expr = expr.x

    # Retrieve the type info (tinfo_t) of the target
    tinfo = target_expr.type

    # Get the size of that type in bytes
    size_in_bytes = tinfo.get_size()

    # If get_size() returns idaapi.BADSIZE (or <= 0), IDA couldn't resolve it
    if size_in_bytes == idaapi.BADSIZE or size_in_bytes <= 0:
        print(f"[-] [{i.ea:X}] Could not resolve the exact size of the underlying type.")
        return None

    return size_in_bytes

founds = []
def add_founds(cf, i):
    result = {
        "ea": i.ea,
        "dstr": i.dstr(),
        'func_name': idc.get_func_name(i.ea),
    }
    
    sz = get_computed_sizeof(cf, i)
    if sz is not None:
        result['block_size'] = sz
        founds.append(result)
        return True

class display_result_t(ida_kernwin.Choose):
    window_title = "SBZ Memset"

    def __init__(self, founds):
        self.founds = founds

        _title = self._find_win_title()

        ida_kernwin.Choose.__init__(
            self,
            _title,
            [ ["Function", 20 | ida_kernwin.CHCOL_FNAME],
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
        return [self.founds[n]["func_name"], 
            hex(self.founds[n]["ea"]), 
            hex(self.founds[n]["block_size"]),
            self.founds[n]["dstr"]
            ]
    
    def OnClose(self):
        self.founds = []

    def OnSelectLine(self, n):
        item_ea = self.founds[n]["ea"]
        func_ea = self.founds[n]["func_name"]
        ea = func_ea if item_ea == BADADDR else item_ea
        ida_kernwin.jumpto(ea)

def main():
    #eas = [here()]
    eas = list(idautils.Functions())

    for ix, ea in enumerate(eas):
        print(f"function {ix+1}/{len(eas)} at {hex(ea)} ...")

        results = hxtb.find_item(ea, lambda cf, i: (q_memset(cf, i) and add_founds(cf, i)))

    display_result = display_result_t(founds)

if __name__ == '__main__':
    main()