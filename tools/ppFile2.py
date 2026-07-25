import os
from io import BytesIO

from dotenv import load_dotenv
load_dotenv()

# 0x70A2ED
special_header = [0x6B, 0x42, 0x6C, 0xA2, 0x72, 0x30, 0x7D, 0x22]

# 0x880680 = g_SBZGlobal
# 0x88DB80 = 0x02

class FileItem:
    name: str
    size: int
    data: bytes | bytearray

    def __init__(self, name: bytes | str, size=0, data=None):
        self.name = name
        self.size = size
        self._data = data

    @property
    def data(self):
        return self._data

    @data.setter
    def data(self, d):
        self._data = b'' if d is None else d

    def save_as_file(self, dst_path):
        if not os.path.exists(dst_path):
            os.makedirs(dst_path, exist_ok=True)
        with open(os.path.join(dst_path, self.name), 'wb') as f:
            f.write(self.data)

    def __str__(self):
        return f'< {self.name} | {self.size} >'

    def __repr__(self):
        return self.__str__()


def bin2ascii(b):
    res = []
    for x in b:
        if x < 0x20 or x > 0x7e:
            res.append('.')
        else:
            res.append(chr(x))
    return "".join(res)

def unmask(b):
    o = bytearray()
    for c in b:
        o.append(~c & 0xFF)
    return o

def decrypt_header_data(data: bytearray) -> bytearray:
    """
    Python implementation of the DecryptHeaderData routine.
    Note: 'data' must be a bytearray to allow in-place modification.

    SBZ VA Address: 0x0070B640
    """
    # Initial state of the key (8-byte rolling key)
    key_state = [
        0xFA, 0x49, 0x7B, 0x1C, 0xF9, 0x4D, 0x83, 0x0A
    ]
    
    # Values added to the state each time that index is accessed
    key_deltas = [
        0x3A, 0xE3, 0x87, 0xC2, 0xBD, 0x1E, 0xA6, 0xFE
    ]

    size = len(data)
    out = bytearray()

    for offset in range(size):
        # key_idx = offset & 7 (Equivalent to offset % 8)
        key_idx = offset & 7
        
        # key_state[key_idx] = LOBYTE(key_deltas) + LOBYTE(key_state)
        # We use & 0xFF to mimic 8-bit unsigned overflow (LOBYTE behavior)
        new_val = (key_deltas[key_idx] + key_state[key_idx]) & 0xFF
        key_state[key_idx] = new_val
        
        # data[offset] ^= LOBYTE(key_state)
        out.append(data[offset] ^ key_state[key_idx])
        
    return out

def decrypt_content_data(data: bytearray) -> bytearray:
    if data[:8] == special_header:
        data = data[8:]

    # SBZ VA Address: 0x4016DB
    key = [
        0x14,0x6F,0x07,0xB8,0x9A,0x0E,0x84,0x44,
        0x59,0x25,0x8E,0x18,0xBC,0x39,0x9E,0x5C,
        0x99,0x7A,0xA0,0x92,0xD4,0xB7,0xBC,0x55,
        0x1E,0x2E,0x88,0x27,0x14,0xA1,0xE6,0x27
    ]
    
    size = len(data)
    out = bytearray()

    # SBZ VA Address: 0x0070A271
    i = 0
    size_aligned = size & ~3
    size_padded  = size & 3
    for offset in range(size_aligned):
        
        key_idx = offset & 0x1f
        out.append(data[offset] ^ key[key_idx])

    for offset in range(size_padded):
        out.append(data[size_aligned + offset])
        
    return out

def unpack(path, s_dir):
    out = []
    dir_, file = os.path.split(path)
    with open(path, 'rb') as f:
        pos = f.tell()
        number = decrypt_header_data(f.read(1))
        format = int.from_bytes(number, 'little')
        print(f"{pos:#08x}: format = {format}")

        pos = f.tell()
        number = decrypt_header_data(f.read(4))
        count = int.from_bytes(number, 'little')
        print(f"{pos:#08x}: count = {count}")

        pos_toc = f.tell()
        full_data = decrypt_header_data(f.read(count * 268))

    
        pos = f.tell()
        number = decrypt_header_data(f.read(4))
        size = int.from_bytes(number, 'little')
        print(f"{pos:#08x}: size = {size}")

        pos = pos_toc
        for i in range(count):
            name = full_data[i * 268 : i * 268 + 260].decode('utf-8').strip('\x00')
            print(f"{pos:#08x}: name[{i}] = {name}")

            pos += 260

            number = full_data[i * 268 + 260 : i * 268 + 264]
            size = int.from_bytes(number, 'little')
            print(f"{pos:#08x}: size[{i}] = {size:#10x}")

            pos += 4

            number = full_data[i * 268 + 264 : i * 268 + 268]
            offset = int.from_bytes(number, 'little')
            print(f"{pos:#08x}: offset[{i}] = {offset:#10x}")

            pos += 4

            f.seek(offset)
            data = decrypt_content_data(f.read(size))

            print(f"{offset:#08x}: data = {bin2ascii(data[:16])}")

            item = FileItem(name, size, data)
            out.append(item)

    s_path = os.path.join(s_dir, f'{os.path.splitext(file)[0]}')
    if not os.path.exists(s_path):
        os.makedirs(s_path, exist_ok=True)
    for i in out:
        print(i, s_path)
        i.save_as_file(s_path)

if __name__ == '__main__':
    appdir = os.getenv('SBZ_APPDIR')
    print(f"SBZ_APPDIR={appdir}")
    unpack(os.path.join(appdir, 'data', 'sb07_00.pp'), os.path.join(appdir, 'out'))