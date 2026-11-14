import idaapi
import idc
import idautils
import ida_nalt
import ida_bytes
import ida_segment
import ida_kernwin
  
idaapi.require('scan_sj')
from scan_sj import get_or_create_encoding, apply_sjis, apply_jpname

idaapi.require('lister')
from lister import get_segm_by_name, list_everything, find_nullterminated, maybe_shiftjis, item_end_is_notnull
# ---------------
ACTION_NAME = "dev_script:shift_j_action"

class MyActionHandler(ida_kernwin.action_handler_t):
    def activate(self, ctx):
        # --- Put your updated logic here ---
        here_set_encoding()
        return 1

    def update(self, ctx):
        return ida_kernwin.AST_ENABLE_ALWAYS

# 1. Always unregister the previous version by name first
ida_kernwin.unregister_action(ACTION_NAME)

# 2. Register the updated version
action_desc = ida_kernwin.action_desc_t(
    ACTION_NAME,
    "Dev Script Shift+J",
    MyActionHandler(),
    "Shift-J"
)

if ida_kernwin.register_action(action_desc):
    print(f"[+] Reloaded {ACTION_NAME} (Shift-J)")
else:
    print(f"[-] Failed to register {ACTION_NAME}")
# -------------
def list_to_camel(words):
    if not words:
        return ""
    # Lowercase the first word, capitalize the rest, and join
    return words[0].lower() + "".join(word.capitalize() for word in words[1:])

def here_set_encoding():
    ea = idc.here()
    flags = ida_bytes.get_full_flags(ea)
    has_dummy_name = ida_bytes.has_dummy_name(flags)
    if not has_dummy_name: return

    end_ea = ea + 512
    nullterminated = find_nullterminated(ea, end_ea)
    if nullterminated == idc.BADADDR:
        print("Could not find nulltermiated")
        return

    sz = nullterminated - ea
    if sz <= 0:
        print("Invalid size")
        return

    if apply_sjis(ea, sz):
        idc.SetType(ea, "const char[];")
        
        names = apply_jpname(ea)
        if names != False:
            new_name = list_to_camel( ["jp"] + names )
            new_name = new_name[:32]
            ida_name.force_name(ea, new_name) 

    
if __name__ == "__main__":
    here_set_encoding()