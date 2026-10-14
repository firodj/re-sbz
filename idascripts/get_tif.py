import ida_typeinf
import ida_bytes
import ida_nalt
import ida_typeinf

def get_c_type(ea):
    # Create an empty tinfo_t container object
    tif = ida_typeinf.tinfo_t()

    # Populate the container with the type at the specified address
    if ida_nalt.get_tinfo(tif, ea):
        print(f"Successfully retrieved type: {str(tif)}")
        print(f"Size of variable: {tif.get_size()} bytes")
        
        # Example: Check if the global variable is a pointer or array
        if tif.is_ptr():
            print("The variable is a pointer.")
        elif tif.is_array():
            print(f"The variable is an array of elements: {str(tif.get_array_element())}")
        if tif.is_funcptr():
            print("is_funcptr")

        flags = ida_typeinf.PRTYPE_1LINE | ida_typeinf.PRTYPE_CPP | ida_typeinf.PRTYPE_TYPE
        type_str = tif._print("", flags)

    else:
        # Fallback: Infer basic type directly from byte allocation size
        size = ida_bytes.get_item_size(ea)
        if size == 1:
            type_str = "uint8_t"
        elif size == 2:
            type_str = "uint16_t"
        elif size == 4:
            type_str = "uint32_t"
        elif size == 8:
            type_str = "uint64_t"
        else:
            type_str = f"char[{size}]"

        print(f"No explicit type information is set at this {ea:x} address")
    
    return type_str

if __name__ == '__main__':
    ea = here()
    type_str = get_c_type(ea)
    print(f"type_str = {type_str}")
