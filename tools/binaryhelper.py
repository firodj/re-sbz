import struct
from io import BytesIO
from dataclasses import dataclass, field
from typing import List, Optional

try:
    import codecs
    _shift_jis = codecs.lookup('shift_jis')
except LookupError:
    _shift_jis = codecs.lookup('cp932')

def red_print(*args, **kwargs):
    print("\033[31m", end='')
    print(*args)
    print("\033[0m", end='')

def _decrypt_name(buf: bytes) -> str:
    if len(buf) < 1:
        return ''
    decrypted = bytes(~b & 0xFF for b in buf)
    decoded_data, consumed = _shift_jis.decode(decrypted)
    result = decoded_data.rstrip('\x00')

    data_1 = len(buf)
    data_2 = len(decoded_data)
    data_3 = len(result)

    test = _encrypt_name(result)
    data_4 = len(test)

    if data_1 != data_4:
        red_print(">>", data_1, data_2, data_3, data_4)
        red_print(">> buf =", repr(buf))
        red_print(">> decoded_data =", repr(decoded_data))

        #raise Exception("data_1 not same with data_4")
    
    return result

def _encrypt_name(name: str) -> bytes:
    if len(name) < 1:
        encrypted = bytearray(1)
    else:
        encoded_data, consumed = _shift_jis.encode(name)
        encrypted = bytearray(encoded_data) + bytearray(1)

    for i in range(len(encrypted)):
        encrypted[i] = ~encrypted[i] & 0xFF
    return bytes(encrypted)

def _encrypt_name_fixed(name: str, length: int) -> bytes:
    encrypted = bytearray(length)
    encoded = _shift_jis.encode(name)
    encrypted[:len(encoded)] = encoded[:length]
    for i in range(len(encrypted)):
        encrypted[i] = ~encrypted[i] & 0xFF
    return bytes(encrypted)


class BinaryReader:
    def __init__(self, stream):
        self._stream = stream
        self._cached = bytes()
    
    def read_bytes(self, *arg) -> bytes:
        if len(arg) == 0:
            res = self._cached + self._stream.read()
            self._cached = bytes()
            return res
        else:
            size = arg[0]
            res = self._cached[:size]
            self._cached = self._cached[size:]
            if len(res) < size:
                res += self._stream.read(size-len(res))
            return res
    
    def read_jpbytes(self, size):
        return _shift_jis.decode(self.read_bytes(size).strip(b'\0'))[0]
    
    def peek_bytes(self, size) -> bytes:
        if len(self._cached) < size:
            self._cached += self._stream.read(size-len(self._cached))
        return self._cached

    def read_byte(self) -> int:
        return struct.unpack('<B', self.read_bytes(1))[0]

    def read_int32(self) -> int:
        return struct.unpack('<i', self.read_bytes(4))[0]

    def read_uint32(self) -> int:
        return struct.unpack('<I', self.read_bytes(4))[0]

    def read_uint16(self) -> int:
        return struct.unpack('<H', self.read_bytes(2))[0]

    def read_single(self) -> float:
        return struct.unpack('<f', self.read_bytes(4))[0]

    def read_uint16_array(self, length: int) -> list:
        return [self.read_uint16() for _ in range(length)]

    def read_uint32_array(self, length: int) -> list:
        return [self.read_uint32() for _ in range(length)]

    def read_name(self, length: Optional[int] = None) -> str:
        if length is not None:
            buf = self.read_bytes(length)
            return _decrypt_name(buf)
        name_len = self.read_int32()
        buf = self.read_bytes(name_len)
        return _decrypt_name(buf)

    def read_string(self):
        r = bytes()
        while True:
            b = self.read_bytes(1)
            if not b:
                break
            r += b
            if b == b'\0':
                break
        return r
    
    def read_jpstring(self):
        return _shift_jis.decode(self.read_string().strip(b'\0'))[0]

    def read_name_skip_1(self) -> str:
        name_len = self.read_int32()
        if name_len == 1:
            discard = self.read_byte()
            return '\0'
        else:
            buf = self.read_bytes(name_len)
            return _decrypt_name(buf)

    def read_color4(self):
        r = abs(self.read_single())
        g = abs(self.read_single())
        b = abs(self.read_single())
        a = abs(self.read_single())
        return (r, g, b, a)

    def read_vector2(self):
        x = self.read_single()
        y = self.read_single()
        return (x, y)

    def read_vector3(self):
        x = self.read_single()
        y = self.read_single()
        z = self.read_single()
        return (x, y, z)

    def read_quaternion(self):
        x = self.read_single()
        y = self.read_single()
        z = self.read_single()
        w = self.read_single()
        return (x, y, z, w)

    def read_matrix(self):
        m0 = self.read_quaternion()
        m1 = self.read_quaternion()
        m2 = self.read_quaternion()
        m3 = self.read_quaternion()
        return (m0, m1, m2, m3)

    def read_to_end(self) -> bytes:
        return self.read_bytes()


class BinaryWriter:
    def __init__(self, stream):
        self._stream = stream

    def write_byte(self, value: int):
        self._stream.write(struct.pack('<B', value))

    def write_bytes(self, data: bytes):
        self._stream.write(data)

    def write_int32(self, value: int):
        self._stream.write(struct.pack('<i', value))

    def write_uint32(self, value: int):
        self._stream.write(struct.pack('<I', value))

    def write_single(self, value: float):
        self._stream.write(struct.pack('<f', value))

    def write_uint16(self, value: int):
        self._stream.write(struct.pack('<H', value))

    def write_name(self, name: str, length: Optional[int] = None):
        if length is not None:
            name_buf = _encrypt_name_fixed(name, length)
            self.write_int32(len(name_buf))
            self.write_bytes(name_buf)
        else:
            name_buf = _encrypt_name(name)
            self.write_int32(len(name_buf))
            self.write_bytes(name_buf)

    def write_name_without_length(self, name: str, length: int):
        name_buf = _encrypt_name_fixed(name, length)
        self.write_bytes(name_buf)

    def write_color4(self, color):
        self.write_single(color[0])
        self.write_single(color[1])
        self.write_single(color[2])
        self.write_single(color[3])

    def write_vector3(self, v):
        self.write_single(v[0])
        self.write_single(v[1])
        self.write_single(v[2])

    def write_quaternion(self, q):
        self.write_single(q[0])
        self.write_single(q[1])
        self.write_single(q[2])
        self.write_single(q[3])

if __name__ == '__main__':
    import json

    buf = b'\xff\xff'
    out = _decrypt_name(buf)

    adict = dict(name=out)

    print(repr(buf))
    print(repr(out))

    print(json.dumps(adict, indent=4, ensure_ascii=False))