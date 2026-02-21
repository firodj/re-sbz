import idautils
import idc
import ida_name
import ida_ua
import ida_funcs

def rename_dlgproc_callbacks():
    """Find all calls to DialogBoxParamA and rename their lpDialogFunc callback
    to DlgProc_{ADDRESS}."""

    ea = idc.get_name_ea_simple("DialogBoxParamA")
    if ea == idc.BADADDR:
        print("DialogBoxParamA not found in imports.")
        return

    print(f"DialogBoxParamA at: {hex(ea)}")

    renamed = 0

    for xref in idautils.XrefsTo(ea):
        call_ea = xref.frm
        mnem = idc.print_insn_mnem(call_ea)
        if mnem != "call":
            continue

        print(f"\nCall at {hex(call_ea)}:")

        # DialogBoxParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam)
        # In x86 stdcall, args pushed right-to-left:
        #   push dwInitParam
        #   push lpDialogFunc   <-- 2nd push before call
        #   push hWndParent
        #   push lpTemplateName
        #   push hInstance
        #   call DialogBoxParamA

        curr = call_ea
        push_count = 0
        dlgproc_ea = None

        for _ in range(30):
            curr = idc.prev_head(curr)
            if curr == idc.BADADDR:
                break

            m = idc.print_insn_mnem(curr)

            if m == "push":
                push_count += 1
                if push_count == 2:  # lpDialogFunc
                    op_type = idc.get_operand_type(curr, 0)
                    op_val = idc.get_operand_value(curr, 0)
                    disasm = idc.generate_disasm_line(curr, 0)
                    print(f"  lpDialogFunc push at {hex(curr)}: {disasm}")

                    if op_type == idc.o_imm:
                        dlgproc_ea = op_val
                    break

            elif m in ("call", "ret", "retn", "jmp"):
                # If we hit another call, the push before it is consumed by
                # that call, so reset the counter.
                if m == "call":
                    push_count = 0
                else:
                    print(f"  Hit {m} at {hex(curr)}, stopping.")
                    break

        if dlgproc_ea is None:
            print("  Could not determine lpDialogFunc address.")
            continue

        old_name = idc.get_name(dlgproc_ea)
        new_name = f"DlgProc_{dlgproc_ea:X}"

        if old_name == new_name:
            print(f"  {hex(dlgproc_ea)} already named {new_name}")
            continue

        if ida_name.set_name(dlgproc_ea, new_name, ida_name.SN_NOWARN):
            print(f"  Renamed {hex(dlgproc_ea)}: {old_name} -> {new_name}")
            renamed += 1
        else:
            print(f"  Failed to rename {hex(dlgproc_ea)}")

    print(f"\nDone. Renamed {renamed} callback(s).")

if __name__ == "__main__":
    rename_dlgproc_callbacks()
