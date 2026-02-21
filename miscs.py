import re

def align_up(ea, align):
    return (ea + align - 1) & ~(align - 1)

def maybe_shiftjis(data_bytes):
    return any((0xA1 <= x <= 0xDF) or (0x81 <= x <= 0x9F) for x in data_bytes)
    #customs = [x for x in data_bytes if (0xA1 <= x and x <= 0xDF)]
    #customs2 = [x for x in data_bytes if (0x81 <= x and x <= 0x9F)]
    #return len(customs) > 0 or len(customs2) > 0

def maybe_ascii(data_bytes):
    if data_bytes[-1] == 0:
        data_bytes = data_bytes[:-1]
    return all(0x20 <= x <= 0x7E or x in [0x0D, 0x0A, 0x09] for x in data_bytes)

def _yara_to_regex(hexstring):
    qmark = False
    for byte in hexstring.split():
        if byte == "??":
            out = ".."
            if not qmark:
                qmark = True
                out = "(" + out
        elif len(byte) == 2 and byte[0] == "?":
             
            out = ".)" + byte[1]
            if not qmark:
                out = "(" + out
                qmark = False
        elif len(byte) == 2 and byte[1] == "?":
            out = byte[0] + "(."
            qmark = True
        else:
            out = byte
            if qmark:
                out = ")" + out
                qmark = False
        
        yield out

    if qmark:
        yield ")"

def yara_to_regex(hexstring):
    return re.compile("".join(_yara_to_regex(hexstring)))

if __name__ == '__main__':
    yara = "55 8b ec 51 89 4d fc 8b 45 fc 8b 4d 08 89 ( 08 | 48 ?? | 88 ?? ?? ?? ?? ) ( 8b 45 fc | ) 8b e5 5d ( c3 | c2 ?? ?? )"
    print(yara_to_regex(yara))
