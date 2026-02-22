"""
create_class_struct.py
======================
Generic IDA Pro Python script to create a C struct for a class by analyzing
its constructor.  Given a constructor address and the target struct name, the
script:

  1. Disassembles the constructor and finds every ADD ECX/EAX/EDX, <imm>
     instruction where 'this' is kept in ECX (thiscall), so it can extract
     every offset that is passed to a sub-constructor.
  2. Decompiles the constructor with Hex-Rays and extracts named sub-calls
     from the pseudocode text as a fallback / for comment annotation.
  3. Builds a flat struct layout with opaque 'char data[N]' members between
     each recognised offset.
  4. Optionally – when you supply 'total_size' – pads the trailing bytes so
     the struct has the exact desired size.
  5. Declares the struct in IDA's type system and applies it to the
     constructor's 'this' parameter.

USAGE
-----
Run from IDA's "File -> Script file…" menu, or paste into the Python console.
Edit the CONFIG section at the bottom before running.

REQUIREMENTS
------------
  * IDA Pro 7.x+ with Hex-Rays decompiler (optional but gives richer output)
  * x86 binary (thiscall – ECX is 'this')
"""

import re
import idaapi
import idc
import idautils
import ida_hexrays
import ida_nalt
import ida_bytes
import ida_funcs


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def get_func_disasm_offsets(func_ea):
    """
    Walk every instruction in the function.  Return a sorted list of
    (offset, call_target_ea, call_target_name) for each CALL instruction
    that is immediately preceded (within 3 instructions) by an
    ADD/LEA that loads THIS+offset into the call register.

    We track the 'this' register conservatively:
      - On entry it is ECX (thiscall)
      - MOV reg, [ebp+var_X] where var_X is the slot that stores ECX on
        entry copies 'this' into another register.
      - ADD reg, imm   ->  offset = imm
      - LEA reg, [other_reg + imm]  -> offset = imm  (if other_reg is this)
    """
    func = ida_funcs.get_func(func_ea)
    if not func:
        print(f"[!] No function at 0x{func_ea:X}")
        return []

    results = []          # list of (offset, callee_ea, callee_name)
    pending_offset = None # last computed offset
    this_slots = set()    # EBP-relative stack slots that hold 'this'
    THIS_REGS = set()     # registers currently holding 'this' (or this+offset)
    offset_regs = {}      # reg -> offset into 'this'

    # ---- first pass: find the stack slot where ECX is saved ----------------
    ecx_slot = None
    ea = func.start_ea
    while ea < func.start_ea + 32:   # look only in prologue
        mnem = idc.print_insn_mnem(ea)
        if mnem == 'MOV':
            op0 = idc.print_operand(ea, 0)
            op1 = idc.print_operand(ea, 1)
            if op1 == 'ecx' and '[ebp' in op0:
                # MOV [ebp+var_X], ecx
                ecx_slot = op0
                break
        ea = idc.next_head(ea, func.end_ea)

    # ---- second pass: track 'this' register and record calls ---------------
    ea = func.start_ea
    while ea < func.end_ea:
        mnem = idc.print_insn_mnem(ea)

        # Reload of 'this' from its stack slot
        if mnem == 'MOV' and ecx_slot:
            op0 = idc.print_operand(ea, 0)   # destination register
            op1 = idc.print_operand(ea, 1)
            if op1 == ecx_slot:
                # reg <- 'this'
                offset_regs[op0] = 0
            elif op1 in offset_regs and '[ebp' not in op1:
                # propagate: reg <- other_reg  (copy)
                offset_regs[op0] = offset_regs[op1]

        # ADD reg, imm  (this += offset)
        if mnem == 'ADD':
            op0 = idc.print_operand(ea, 0)
            if op0 in offset_regs:
                imm = idc.get_operand_value(ea, 1)
                if 0 < imm < 0x10000000:   # sanity: positive & not huge
                    offset_regs[op0] = imm   # new offset

        # LEA reg, [other+imm]
        if mnem == 'LEA':
            op0 = idc.print_operand(ea, 0)
            op1_raw = idc.print_operand(ea, 1)   # e.g. "[ecx+1CB34h]"
            m = re.search(r'\[(\w+)\+([0-9A-Fa-f]+h?)\]', op1_raw)
            if m:
                base = m.group(1)
                imm_str = m.group(2).rstrip('h')
                imm = int(imm_str, 16) if imm_str else 0
                if base in offset_regs and 0 < imm < 0x10000000:
                    offset_regs[op0] = imm

        # CALL  – record (offset, callee)
        if mnem == 'CALL':
            # by convention for thiscall: callee receives ECX
            off = offset_regs.get('ecx', None)
            if off is not None and off > 0:
                callee = idc.get_operand_value(ea, 0)
                name = idc.get_name(callee) or f"sub_{callee:X}"
                results.append((off, callee, name))
            pending_offset = None

        ea = idc.next_head(ea, func.end_ea)

    return sorted(set(results), key=lambda x: x[0])


def get_func_offsets_from_pseudocode(func_ea):
    """
    Decompile with Hex-Rays and extract (offset, sub_name) pairs from the
    pseudocode by looking for patterns like:
      sub_XXXXXX(this + 0x1234)
      sub_XXXXXX((char *)this + 300)
      SomeClass::ctor((char *)this + NNN)
    Returns list of (offset_int, name_str).
    """
    results = []
    try:
        cfunc = ida_hexrays.decompile(func_ea)
        if not cfunc:
            return results
        code = str(cfunc)
        # Pattern: function_name(anything + decimal_or_hex_number)
        patterns = [
            r'(\w[\w:~]+)\s*\(\s*(?:\([^)]*\)\s*)?\bthis\s*\+\s*(\d+)\)',
            r'(\w[\w:~]+)\s*\(\s*(?:_DWORD\s*\*\s*\*?\s*|\S*\s*)\bthis\s*\+\s*(\d+)\)',
        ]
        for pat in patterns:
            for m in re.finditer(pat, code):
                name = m.group(1)
                off = int(m.group(2))
                results.append((off, name))
    except Exception as e:
        print(f"[W] Hex-Rays decompilation failed: {e}")
    return sorted(set(results), key=lambda x: x[0])


def get_eh_vector_calls(func_ea):
    """
    Parse Hex-Rays pseudocode for 'eh vector constructor iterator' calls.
    Returns list of (offset, element_size, count, ctor_name).
    Pattern:
      `eh vector constructor iterator'(this + OFF, ELEM_SZ, COUNT, ctor, dtor)
    """
    results = []
    try:
        cfunc = ida_hexrays.decompile(func_ea)
        code = str(cfunc)
        pat = (r"eh vector constructor iterator'\s*\(\s*"
               r"(?:[^,]*this\s*\+\s*(\d+)|this\s*\+\s*(\d+))\s*,"   # offset
               r"\s*(0x[0-9A-Fa-f]+|\d+)u?\s*,"                        # elem size
               r"\s*(\d+)\s*,"                                          # count
               r"\s*\(void[^)]*\)\s*\(void \*\)\)(\w[\w:~]*)")         # ctor name
        for m in re.finditer(pat, code):
            off  = int(m.group(1) or m.group(2))
            esz  = int(m.group(3), 0)
            cnt  = int(m.group(4))
            name = m.group(5)
            results.append((off, esz, cnt, name))
    except Exception as e:
        pass
    return results


def parse_constructor_layout(func_ea):
    """
    Combine disassembly + Hex-Rays to build a list of StructMember entries.

    Returns a list of dicts:
     { 'offset': int, 'name': str, 'count': int, 'elem_size': int,
       'is_array': bool, 'callee_ea': int }

    'elem_size' may be 0 if unknown (will be determined by gap to next
    known member).
    """
    members = {}   # offset -> dict

    # 1. Get array ctors from pseudocode  (most reliable for array sizes)
    for off, esz, cnt, name in get_eh_vector_calls(func_ea):
        members[off] = {
            'offset': off, 'name': name, 'count': cnt,
            'elem_size': esz, 'is_array': True, 'callee_ea': 0,
            'total_known': esz * cnt
        }

    # 2. Get individual sub-constructor calls from disassembly
    for off, callee, name in get_func_disasm_offsets(func_ea):
        if off not in members:
            members[off] = {
                'offset': off, 'name': name, 'count': 1,
                'elem_size': 0, 'is_array': False, 'callee_ea': callee,
                'total_known': 0
            }

    return sorted(members.values(), key=lambda m: m['offset'])


def declare_opaque_type(type_name, size):
    """Create/overwrite an opaque struct of 'size' bytes."""
    decl = f"struct {type_name} {{ char data[{size}]; }};"
    tif = idaapi.tinfo_t()
    ok  = bool(idaapi.parse_decl(tif, None, decl, idaapi.PT_SIL))
    if ok:
        tif.set_named_type(idaapi.get_idati(), type_name, idaapi.NTF_REPLACE)
    return ok


def build_and_apply_struct(
        func_ea,
        struct_name,
        total_size=0,
        vftable_name=None,
        prefix=None):
    """
    Main entry point.

    Parameters
    ----------
    func_ea      : int  – constructor address
    struct_name  : str  – desired struct name (e.g. "SBShopScene")
    total_size   : int  – known total allocation size; 0 = auto
    vftable_name : str  – exact vftable symbol (optional, for documentation)
    prefix       : str  – prefix for opaque sub-type names; defaults to struct_name

    Returns the declared tinfo_t or None on failure.
    """
    if prefix is None:
        prefix = struct_name

    print(f"\n[*] Analysing constructor 0x{func_ea:X} for '{struct_name}'")
    members = parse_constructor_layout(func_ea)

    # Determine the offset of the first sub-object after vftable
    offsets_for_sizes = [m['offset'] for m in members]
    if total_size:
        offsets_for_sizes.append(total_size)

    # -----------------------------------------------------------------------
    # Build the C declaration line by line
    # -----------------------------------------------------------------------
    lines = [f"struct {struct_name} {{"]
    cursor = 0

    # vftable
    lines.append(f"    void* vftable;  // 0x{cursor:04X}")
    cursor = 4

    for i, m in enumerate(members):
        off  = m['offset']
        name = m['name']
        cnt  = m['count']
        esz  = m['elem_size']

        # ---- padding / gap before this member ------------------------------
        if off > cursor:
            gap = off - cursor
            lines.append(f"    char _pad_{cursor:X}[{gap}];  // 0x{cursor:04X} gap ({gap} bytes)")
            cursor = off

        # ---- determine element size -----------------------------------------
        if esz == 0:
            # infer from distance to next known offset
            next_off = (offsets_for_sizes[offsets_for_sizes.index(off) + 1]
                        if off in offsets_for_sizes and
                           offsets_for_sizes.index(off) + 1 < len(offsets_for_sizes)
                        else 0)
            if next_off:
                esz = next_off - off
            else:
                esz = 4   # fallback to DWORD

        # ---- create opaque sub-type -----------------------------------------
        clean = re.sub(r'[^A-Za-z0-9_]', '_', name)
        sub_type = f"{prefix}_{clean}_{esz}"
        declare_opaque_type(sub_type, esz)

        # ---- emit field declaration -----------------------------------------
        if cnt > 1:
            lines.append(
                f"    {sub_type} {clean.lower()}_{off:X}[{cnt}];  "
                f"// 0x{off:04X}  ({cnt} x 0x{esz:X} = 0x{cnt*esz:X} bytes)")
        else:
            lines.append(
                f"    {sub_type} {clean.lower()}_{off:X};  "
                f"// 0x{off:04X}  (0x{esz:X} bytes)")

        cursor = off + esz * cnt

    # ---- trailing gap to reach total_size -----------------------------------
    if total_size and cursor < total_size:
        gap = total_size - cursor
        lines.append(f"    char _tail_{cursor:X}[{gap}];  // 0x{cursor:04X} tail padding")
        cursor = total_size

    lines.append("};")
    decl = "\n".join(lines)

    print(f"\n[*] Generated declaration ({cursor} bytes = 0x{cursor:X}):\n")
    print(decl)

    # -----------------------------------------------------------------------
    # Parse & register the struct
    # -----------------------------------------------------------------------
    tif = idaapi.tinfo_t()
    flags = idaapi.PT_SIL | idaapi.PT_REPLACE
    ok = bool(idaapi.parse_decl(tif, None, decl + ";", flags))
    if not ok:
        print(f"\n[!] parse_decl failed — trying without PT_REPLACE flag")
        ok = bool(idaapi.parse_decl(tif, None, decl + ";", idaapi.PT_SIL))

    if not ok:
        print("[!] Failed to declare struct.  Check the declaration above.")
        return None

    tif.set_named_type(idaapi.get_idati(), struct_name, idaapi.NTF_REPLACE)
    print(f"\n[+] Struct '{struct_name}' registered in IDA (size = {tif.get_size()} bytes)")

    # -----------------------------------------------------------------------
    # Apply type to the constructor's 'this' parameter
    # -----------------------------------------------------------------------
    func_decl = (f"{struct_name} * __thiscall {struct_name}__ctor"
                 f"({struct_name} *this, int a2);")
    ftif = idaapi.tinfo_t()
    ok2 = bool(idaapi.parse_decl(ftif, None, func_decl, idaapi.PT_SIL))
    if ok2:
        applied = idaapi.apply_tinfo(func_ea, ftif, idaapi.TINFO_DEFINITE)
        if applied:
            print(f"[+] Applied '{struct_name}*' to constructor at 0x{func_ea:X}")
        else:
            print(f"[!] apply_tinfo returned False for 0x{func_ea:X}")
    else:
        print(f"[!] Could not parse function type declaration: {func_decl}")

    return tif


# ===========================================================================
# CONFIG  –  edit this section and run the script
# ===========================================================================
if __name__ == "__main__":

    # ---- Example 1: SBShopScene -------------------------------------------
    build_and_apply_struct(
        func_ea     = 0x512870,          # constructor address
        struct_name = "SBShopScene",     # desired struct name
        total_size  = 0x1CE1C,           # from allocation before the call
        vftable_name= "SBShopScene::`vftable'",
    )

    # ---- Example 2: SBNightShopScene  (uncomment to use) ------------------
    # build_and_apply_struct(
    #     func_ea     = 0x5081B0,
    #     struct_name = "SBNightShopScene",
    #     total_size  = 0,                # unknown; will auto-detect from members
    #     vftable_name= "SBNightShopScene::`vftable'",
    # )
