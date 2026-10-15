import ida_typeinf
import ida_bytes
import ida_nalt
import ida_typeinf

def get_c_type(ea):
    # Create an empty tinfo_t container object
    tif = ida_typeinf.tinfo_t()

    result = dict(
        is_ptr=False,
        is_array=False,
        is_funcptr=False,
        is_struct=False,
        is_typeref=False,
        size=None,
        type_str=None,
        tif=None,
        target=None,
        element_type=None,
        element_count=None,
    )

    # Populate the container with the type at the specified address
    if ida_nalt.get_tinfo(tif, ea):
        #print(f"Successfully retrieved type: {str(tif)}")
        result['size'] = tif.get_size()
        
        # Example: Check if the global variable is a pointer or array
        if tif.is_ptr():
            result['is_ptr'] = True
            if tif.is_typeref:
                result['target'] = tif.get_pointed_object()

        if  tif.is_array():
            result['is_array'] = tif.is_array()
            result['element_type'] = str(tif.get_array_element())
            result['element_count'] = str(tif.get_array_nelems())

        result['is_funcptr'] = tif.is_funcptr()
        result['is_struct'] = tif.is_struct()
        result['is_typeref'] = tif.is_typeref()
        # get_udt_nmembers
        # is_udt
        # get_udm(n)[1].name

        flags = ida_typeinf.PRTYPE_1LINE | ida_typeinf.PRTYPE_CPP | ida_typeinf.PRTYPE_TYPE
        result['type_str'] = tif._print("", flags)
        result['tif'] = tif

        
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

        #print(f"No explicit type information is set at this {ea:x} address")
        result['type_str'] = type_str
        result['size'] = size
    
    return result

if __name__ == '__main__':
    ea = here()
    type_result = get_c_type(ea)
    print(f"type_result = {type_result}")
    globals()['type_result'] = type_result

