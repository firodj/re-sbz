import idc
import ida_kernwin
import ida_ua
import ida_idp
import ida_xref
import ida_funcs

is_func = lambda ea: (ea == ida_funcs.get_func(ea).start_ea if ida_funcs.get_func(ea) else False)

def list_code_xrefs_from(ea):
    items = []

    # Get current cursor address
    print(f"\n[ Validating Xrefs originating from: {hex(ea)} ]")
    
    # Counter for references found
    count = 0
    
    # Iterate over all references originating FROM ea
    # ida_xref.XREF_ALL gets all references, but we filter for code references
    xref = ida_xref.xrefblk_t()
    
    # Initialize the first reference
    success = xref.first_from(ea, ida_xref.XREF_ALL)
    
    while success:
        target_ea = xref.to
        # Check if the reference is a code reference (Call, Jump, Flow)
        if xref.iscode and is_func(target_ea):            
            # Get the name of the target if it exists
            target_name = idc.get_func_name(target_ea) or idc.get_name(target_ea) or "sub_%X" % target_ea
            
            # Map reference type to a readable string
            ref_type = "Unknown"
            if xref.type == ida_xref.fl_CN: ref_type = "Computed Near Call (Indirect)"
            elif xref.type == ida_xref.fl_CF: ref_type = "Computed Far Call"
            elif xref.type == ida_xref.fl_JN: ref_type = "Computed Near Jump"
            elif xref.type == ida_xref.fl_F:  ref_type = "Ordinary Flow"
            
            print(f"  -> Target: {hex(target_ea)} ({target_name}) | Type: {ref_type}")
            items.append([hex(target_ea), target_name, ref_type])
            count += 1
            
        # Move to the next reference
        success = xref.next_from()
        
    if count == 0:
        print("  [-] No outward code cross-references found for this instruction.")
    else:
        print(f"[+] Total outbound targets resolved: {count}")

    return items

def add_indirect_call(ea, target_ea):
    idc.add_cref(ea, target_ea, idc.fl_CN)
    print(f"[+] Added Xref: {hex(ea)} -> {hex(target_ea)}")

def is_indirect_call(ea):
    # 1. Ensure it's an instruction
    if not idc.is_code(idc.get_full_flags(ea)):
        return None
        
    # Decode the instruction at the given address
    insn = ida_ua.insn_t()
    if not ida_ua.decode_insn(insn, ea):
        return None
        
    # 2. Check if the instruction feature is a call
    # ida_idp.CF_CALL detects call instructions across x86/x64, ARM, MIPS, etc.
    if (insn.get_canon_feature() & ida_idp.CF_CALL) == 0:
        return None
        
    # 3. Check the first operand (Op1) to see if it's indirect
    op = insn.ops[0]
    
    # Common indirect operand types:
    # o_reg  = Call register (e.g., call eax)
    # o_mem  = Call memory pointer (e.g., call dword ptr [0x401000])
    # o_phrase / o_displ = Call register with offset (e.g., call [rax+28h] or call [rbx])
    indirect_types = [
        ida_ua.o_reg, 
        ida_ua.o_mem, 
        ida_ua.o_phrase, 
        ida_ua.o_displ
    ]
    
    if op.type in indirect_types:
        return True
        
    return False

"""Launches the window and binds custom control buttons."""
# Create an embedded action structure for adding refs
class AddTargetAction(ida_kernwin.action_handler_t):

    @staticmethod
    def compose_action_name(ea):
        return f"xref_manager:add_target_{ea:X}"

    def __init__(self, chooser_instance):
        ida_kernwin.action_handler_t.__init__(self)
        self.chooser = chooser_instance

    def activate(self, ctx):
        # Prompt user for a name or target address destination
        input_str = ida_kernwin.ask_str("", 0, "Enter target address (e.g. 0x140005678) or name:")
        if input_str:
            try:
                # Attempt to resolve address if passed as hex string or plain label name
                if input_str.lower().startswith("0x"):
                    target_ea = int(input_str, 16)
                else:
                    target_ea = idc.get_name_ea_simple(input_str)
                    if target_ea == idc.BADADDR:
                        target_ea = int(input_str, 16)
                
                if target_ea != idc.BADADDR:
                    # Default to Computed Near Call (fl_CN) for indirect calls
                    add_indirect_call(self.chooser.ea, target_ea)

                    # Refresh chooser view instantly
                    self.chooser.populate_items()
                    self.chooser.Refresh()
                else:
                    print("[-] Error: Invalid destination target.")
            except ValueError:
                print("[-] Error: Parsing target failed.")
        return 1

    def update(self, ctx):
        return ida_kernwin.AST_ENABLE_ALWAYS

class ShowXrefManagerAction(ida_kernwin.action_handler_t):
    ACTION_NAME = 'xref_manager:show'

    def __init__(self):
        ida_kernwin.action_handler_t.__init__(self)

    def activate(self, ctx):
        current_ea = ctx.cur_ea # ida_kernwin.get_screen_ea()
        if current_ea == idc.BADADDR:
            print("[-] Select a valid address first.")
            return

        caller = is_indirect_call(current_ea)
        if caller is None:
            print("[-] Not a call instruction.")
            return

        # list_code_xrefs_from(current_ea)

        title = f"Outbound Xrefs from {hex(current_ea)}"
        chooser = XrefManagerChooser(title, current_ea)
        chooser.show_ui()

        # print("context fields: %s" % dir(ctx))
        # print(" cur_ea %08X" % ctx.cur_ea)
        # print(" cur_value: %08X" % ctx.cur_value)
        # print(" cur_extracted_ea %08X" % ctx.cur_extracted_ea)
        return 1

    # You can implement update(), to inform IDA when:
    #  * your action is enabled
    #  * update() should queried again
    # E.g., returning 'ida_kernwin.AST_ENABLE_FOR_WIDGET' will
    # tell IDA that this action is available while the
    # user is in the current widget, and that update()
    # must be queried again once the user gives focus
    # to another widget.
    #
    # For example, the following update() implementation
    # will let IDA know that the action is available in
    # "IDA View-*" views, and that it's not even worth
    # querying update() anymore until the user has moved
    # to another view..
    def update(self, ctx):
        return ida_kernwin.AST_ENABLE_FOR_WIDGET if ctx.widget_type in (ida_kernwin.BWN_DISASM, ida_kernwin.BWN_PSEUDOCODE) else ida_kernwin.AST_DISABLE_FOR_WIDGET

class XrefManagerChooser(ida_kernwin.Choose):
    def __init__(self, title, ea):
        # Define the table columns: [Title, Width, Alignment]
        # Width is approx character counts; negative means left-aligned
        ida_kernwin.Choose.__init__(
            self,
            title,
            [
                ["Target Address", 15 | ida_kernwin.Choose.CHCOL_HEX],
                ["Function/Label Name", 30],
                ["Xref Type", 25]
            ],
            flags=ida_kernwin.Choose.CH_MODAL
        )
        self.ea = ea
        self.populate_items()

    def populate_items(self):
        self.items = list_code_xrefs_from(self.ea)

    def OnGetSize(self):
        """Returns total row count for the UI list."""
        return len(self.items)

    def OnGetLine(self, n):
        """Returns row string elements based on column configurations."""
        return self.items[n]

    def OnSelectLine(self, n):
        """Double-clicking a row jumps to that target in the disassembly."""
        target_str = self.items[n][0]
        target_ea = int(target_str, 16)
        ida_kernwin.jumpto(target_ea)

    def OnRefresh(self, n):
        """Handles updating list view if underlying data changes."""
        self.populate_items()
        return [ida_kernwin.Choose.ALL_CHANGED, n]

    def show_ui(self):
        act_name = AddTargetAction.compose_action_name(self.ea)

        if ida_kernwin.unregister_action(act_name):
            ida_kernwin.msg("Unregistered older action %s.\n" % (act_name,))
        
        action_desc = ida_kernwin.action_desc_t(
            act_name,                    # Action unique identifier
            'Add Target Destination',    # Text displayed on the button UI
            AddTargetAction(self),           # Target execution instance
        )
        if ida_kernwin.register_action(action_desc):
            ida_kernwin.msg("Registered new action %s.\n" % (act_name,))

        # Render execution view window
        ok = self.Show(False) >= 0
        if ok:
            ida_kernwin.attach_action_to_popup(self.GetWidget(), None, act_name)
        return ok

my_hooks = None

def main():
    global my_hooks

    act_name = ShowXrefManagerAction.ACTION_NAME

    action_desc = ida_kernwin.action_desc_t(
        act_name,                      # Action unique identifier
        'Show Xref Manager',        # Text displayed on the button UI
        ShowXrefManagerAction(),           # Target execution instance
    )
            
    if ida_kernwin.register_action(action_desc):
        ida_kernwin.msg("Registered new action %s.\n" % (act_name,))

        # Insert the action in the menu
        if ida_kernwin.attach_action_to_menu("Edit/Export data", act_name, ida_kernwin.SETMENU_APP):
            print("Attached to menu afer Edit/Export data")
        else:
            print("Failed attaching to menu.")

        # We will also want our action to be available in the context menu
        # for the "IDA View-A" widget.
        #
        # To do that, we could in theory retrieve a reference to "IDA View-A", and
        # then request to "permanently" attach the action to it, using something
        # like this:
        #   ida_kernwin.attach_action_to_popup(ida_view_a, None, act_name, None)
        #
        # but alas, that won't do: widgets in IDA are very "volatile", and
        # can be deleted & re-created on some occasions (e.g., starting a
        # debugging session), and our efforts to permanently register our
        # action on "IDA View-A" would be annihilated as soon as "IDA View-A"
        # is deleted.
        #
        # Instead, we can opt for a different method: attach our action on-the-fly,
        # when the popup for "IDA View-A" is being populated, right before
        # it is displayed.
        class Hooks(ida_kernwin.UI_Hooks):
            def finish_populating_widget_popup(self, widget, popup):
                # We'll add our action to all "IDA View-*"s.
                # If we wanted to add it only to "IDA View-A", we could
                # also discriminate on the widget's title:
                #
                #  if ida_kernwin.get_widget_title(widget) == "IDA View-A":
                #      ...
                #
                print("widget", ida_kernwin.get_widget_type(widget))

                if ida_kernwin.get_widget_type(widget) in (ida_kernwin.BWN_DISASM, ida_kernwin.BWN_PSEUDOCODE):
                    ida_kernwin.attach_action_to_popup(widget, popup, act_name, None)

        my_hooks = Hooks()
        my_hooks.hook()
    else:
        print("Action found; unregistering.")
        # No need to call detach_action_from_menu(); it'll be
        # done automatically on destruction of the action.
        if ida_kernwin.unregister_action(act_name):
            print("Unregistered %s.\n" % (act_name,))
        else:
            print("Failed to unregister action.")

        if my_hooks is not None:
            my_hooks.unhook()
            my_hooks = None    

if __name__ == "__main__":
    main()
