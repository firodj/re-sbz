import idautils
import idc
import ida_name
import ida_funcs

def rename_wndproc_callbacks():
    """Find all calls to RegisterClassExA and rename their lpfnWndProc callback
    to WndProc_{ADDRESS}."""

    ea = idc.get_name_ea_simple("RegisterClassExA")
    if ea == idc.BADADDR:
        print("RegisterClassExA not found in imports.")
        return

    print(f"RegisterClassExA at: {hex(ea)}")

    renamed = 0

    for xref in idautils.XrefsTo(ea):
        call_ea = xref.frm
        mnem = idc.print_insn_mnem(call_ea)
        if mnem != "call":
            continue

        print(f"\nCall at {hex(call_ea)}:")

        func = ida_funcs.get_func(call_ea)
        if not func:
            print("  Not inside a function, skipping.")
            continue

        # WNDCLASSEXA.lpfnWndProc is at offset 0x8 in the struct.
        # Look backwards from the call for an instruction that writes an
        # immediate (offset to a function) into the lpfnWndProc field.
        # Patterns:
        #   mov dword ptr [reg+8], offset sub_XXX
        #   mov [ebp+var.lpfnWndProc], offset sub_XXX
        curr = call_ea
        wndproc_ea = None

        for _ in range(100):
            curr = idc.prev_head(curr)
            if curr < func.start_ea or curr == idc.BADADDR:
                break

            disasm = idc.generate_disasm_line(curr, 0)

            # Check for lpfnWndProc field name or +8] pattern
            if "lpfnWndProc" in disasm or "+8]" in disasm:
                op1_type = idc.get_operand_type(curr, 1)
                op1_val = idc.get_operand_value(curr, 1)

                # Only interested in immediate values (direct function offsets)
                if op1_type == idc.o_imm and 0x400000 <= op1_val < 0x800000:
                    wndproc_ea = op1_val
                    print(f"  Found lpfnWndProc at {hex(curr)}: {disasm}")
                    break
                else:
                    # Register or indirect — likely DefWindowProcA or similar
                    print(f"  Found lpfnWndProc at {hex(curr)}: {disasm}")
                    print(f"    Not an immediate offset (type={op1_type}), skipping.")
                    break

        if wndproc_ea is None:
            print("  Could not determine lpfnWndProc address.")
            continue

        old_name = idc.get_name(wndproc_ea)
        new_name = f"WndProc_{wndproc_ea:X}"

        if old_name == new_name:
            print(f"  {hex(wndproc_ea)} already named {new_name}")
            continue

        if old_name and not old_name.startswith("sub_") and not old_name.startswith("unknown_"):
            print(f"  {hex(wndproc_ea)} already has name '{old_name}', skipping.")
            continue

        if ida_name.set_name(wndproc_ea, new_name, ida_name.SN_NOWARN):
            print(f"  Renamed {hex(wndproc_ea)}: {old_name} -> {new_name}")
            renamed += 1
        else:
            print(f"  Failed to rename {hex(wndproc_ea)}")

    print(f"\nDone. Renamed {renamed} callback(s).")

if __name__ == "__main__":
    rename_wndproc_callbacks()
