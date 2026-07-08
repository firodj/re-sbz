import ida_name
import ida_typeinf
import ida_hexrays

def create_Tree_node_struct():
    struct_name = "std__Tree_node"

    #   +0x00  _Left   (sub_49A9A0 = +0x14)
    #   +0x04  _Parent (sub_7259E0 = +0x08)
    #   +0x08  _Right  (unknown_libname_21 = +0x00, sub_728210 = +0x04)
    #   +0x0C  _Value
    struct_str = """
struct std__Tree_node {
    int _Left;
    int _Parent;
    int _Right;
    int _Value;
};
    """
    til = ida_typeinf.get_idati()
    tif = til.get_named_type(struct_name)

    if tif is not None:
        print(f"struct {struct_name} already exists in til: {til}")
        return tif

    tif = ida_typeinf.tinfo_t(struct_str)

    # Persist it to the "Local types" type library
    tif.set_named_type(til, struct_name)

    return tif

def get_cfunc(ea):
    cfunc = ida_hexrays.decompile(ea)
    return cfunc

def get_this_arg(cfunc):
    for lvar in cfunc.lvars:
        if lvar.name == "this" and lvar.is_arg_var:
            return lvar

def refresh_func(cfunc):
    cfunc.refresh_func_ctext()

def set_names():
    ida_name.set_name(0x4481F0, "std_Tree_erase_range")
    ida_name.set_name(0x448290, "std_Tree_clear")
    ida_name.set_name(0x4482F0, "std_Tree_erase_all_nodes")
    ida_name.set_name(0x4493D0, "std_Tree_iterator_next")
    ida_name.set_name(0x4D3E00, "std_Tree_get_header_value")
    ida_name.set_name(0x629FD0, "std_iterator_not_equal")
    ida_name.set_name(0x449FF0, "std_Tree_iterator_increment")
    ida_name.set_name(0x44A3C0, "std_Tree_iterator_increment_impl")
    ida_name.set_name(0x4D5D50, "std_Tree_get_rightmost")
    ida_name.set_name(0x726540, "std_Tree_get_leftmost")
    ida_name.set_name(0x49AAB0, "std_Tree_get_header_parent")
    ida_name.set_name(0x7259E0, "std_Tree_node_get_parent")
    ida_name.set_name(0x728210, "std_Tree_node_get_right")
    ida_name.set_name(0x49A9A0, "std_Tree_node_get_left")

def try_set_this_type():
    tif = create_Tree_node_struct()
    cfunc = get_cfunc(0x448290)
    lvar = get_this_arg(cfunc)

    if lvar.tif.is_ptr():
        pointed_tif = lvar.tif.get_pointed_object()
        if pointed_tif.get_type_name() != tif.get_type_name():
            change = True
    else:
        change = True

    if change:
        new_type = ida_typeinf.tinfo_t()
        new_type.create_ptr(tif)
        lvar.set_final_lvar_type(new_type)

        lvinf = ida_hexrays.lvar_uservec_t()
        lvinf.keep_info(lvar)
        ida_hexrays.save_user_lvar_settings(cfunc.entry_ea, lvinf)

        refresh_func(cfunc)

if __name__ == "__main__":
    set_names()

