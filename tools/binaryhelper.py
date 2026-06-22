import struct
from io import BytesIO
from dataclasses import dataclass, field
from typing import List, Optional

try:
    import codecs
    _shift_jis = codecs.lookup('shift_jis')
except LookupError:
    _shift_jis = codecs.lookup('cp932')


def _decrypt_name(buf: bytes) -> str:
    if len(buf) < 1:
        return ''
    decrypted = bytes(~b & 0xFF for b in buf)
    decoded_data, consumed = _shift_jis.decode(decrypted)
    return decoded_data.rstrip('\x00')


class BinaryReader:
    def __init__(self, stream):
        self._stream = stream

    def read_byte(self) -> int:
        return struct.unpack('<B', self._stream.read(1))[0]

    def read_bytes(self, count: int) -> bytes:
        return self._stream.read(count)

    def read_int32(self) -> int:
        return struct.unpack('<i', self._stream.read(4))[0]

    def read_uint32(self) -> int:
        return struct.unpack('<I', self._stream.read(4))[0]

    def read_uint16(self) -> int:
        return struct.unpack('<H', self._stream.read(2))[0]

    def read_single(self) -> float:
        return struct.unpack('<f', self._stream.read(4))[0]

    def read_uint16_array(self, length: int) -> list:
        return [self.read_uint16() for _ in range(length)]

    def read_name(self, length: Optional[int] = None) -> str:
        if length is not None:
            buf = self.read_bytes(length)
            return _decrypt_name(buf)
        name_len = self.read_int32()
        buf = self.read_bytes(name_len)
        return _decrypt_name(buf)
    
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

    def read_to_end(self) -> bytes:
        return self._stream.read()

