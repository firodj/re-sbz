import idautils
import idc
import ida_name
import ida_funcs

def rename_thread_callbacks():
    """Find all calls to CreateThread and _beginthreadex, and rename their
    thread start routine callbacks to ThreadProc_{ADDRESS}."""

    apis = [
        # CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId)
        # _beginthreadex(security, stack_size, start_address, arglist, initflag, thrdaddr)
        # In both APIs, the start address is arg 3 (0-indexed),
        # which is pushed as the 4th push before the call (right-to-left).
        {"name": "CreateThread", "callback_push_index": 4},
        {"name": "__beginthreadex", "callback_push_index": 4},
    ]

    renamed = 0

    for api in apis:
        ea = idc.get_name_ea_simple(api["name"])
        if ea == idc.BADADDR:
            print(f"{api['name']}: not found, skipping.")
            continue

        print(f"\n{api['name']} at {hex(ea)}")

        seen = set()
        for xref in idautils.XrefsTo(ea):
            call_ea = xref.frm
            if call_ea in seen:
                continue
            seen.add(call_ea)

            mnem = idc.print_insn_mnem(call_ea)
            if mnem != "call":
                continue

            print(f"\n  Call at {hex(call_ea)}:")

            # Walk backwards to find the push that corresponds to lpStartAddress
            curr = call_ea
            push_count = 0
            threadproc_ea = None

            for _ in range(30):
                curr = idc.prev_head(curr)
                if curr == idc.BADADDR:
                    break

                m = idc.print_insn_mnem(curr)

                if m == "push":
                    push_count += 1
                    if push_count == api["callback_push_index"]:
                        op_type = idc.get_operand_type(curr, 0)
                        op_val = idc.get_operand_value(curr, 0)
                        disasm = idc.generate_disasm_line(curr, 0)
                        print(f"    Callback push at {hex(curr)}: {disasm}")

                        if op_type == idc.o_imm:
                            threadproc_ea = op_val
                        break

                elif m in ("ret", "retn", "jmp"):
                    print(f"    Hit {m}, stopping")
                    break
                elif m == "call":
                    # Reset push count — those pushes were consumed by this call
                    push_count = 0

            if threadproc_ea is None:
                print("    Could not determine callback address.")
                continue

            old_name = idc.get_name(threadproc_ea)
            new_name = f"ThreadProc_{threadproc_ea:X}"

            if old_name == new_name:
                print(f"    {hex(threadproc_ea)} already named {new_name}")
                continue

            if old_name and not old_name.startswith("sub_") and not old_name.startswith("unknown_"):
                print(f"    {hex(threadproc_ea)} has custom name '{old_name}', skipping.")
                continue

            if ida_name.set_name(threadproc_ea, new_name, ida_name.SN_NOWARN):
                print(f"    Renamed {hex(threadproc_ea)}: {old_name} -> {new_name}")
                renamed += 1
            else:
                print(f"    Failed to rename {hex(threadproc_ea)}")

    print(f"\nDone. Renamed {renamed} callback(s).")

if __name__ == "__main__":
    rename_thread_callbacks()
