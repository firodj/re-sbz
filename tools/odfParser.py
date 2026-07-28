import struct
from io import BytesIO
from dataclasses import dataclass, field
from typing import List, Optional
from binaryhelper import BinaryReader, BinaryWriter, red_print
import sys

try:
    import codecs
    _shift_jis = codecs.lookup('shift_jis')
except LookupError:
    _shift_jis = codecs.lookup('cp932')


class OdfParser:
    def __init__(self, stream):
        reader = BinaryReader(stream)

        header = "オリジナル☆フォーマット%c%c%c" % (13, 10, 0)
        sjis, _ = _shift_jis.encode(header)
        hdr = reader.read_bytes(len(sjis))
        
        if hdr == sjis:
            print(_shift_jis.decode(hdr))
            self.parse_content(reader)

        self.parse_tail(reader)
        
    def parse_content(self, reader):
        ender = "ファイルは終了"
        sjis_end, _ = _shift_jis.encode(ender)
        chunk_end = sjis_end[0:4]

        chunk = reader.read_bytes(4)
        while True:
            if len(chunk) < 4:
                rest_chunk = reader.read_bytes(4-len(chunk))
                if not rest_chunk:
                    break
                chunk += rest_chunk
                
            match chunk[0:4]:
                case b'FRAM':
                    chunk = b''
                    self.parse_fram(reader)
                case b'MESH':
                    chunk = b''
                    print('MESH')
                case b'MORP':
                    chunk = b''
                    print("MORP")
                case b'ANIM':
                    chunk = b''
                    print("ANIM")
                case b'3DON':
                    chunk = b''
                    print('3DON')
                case _:
                    if chunk == chunk_end:
                        chunk += reader.read_bytes(len(sjis_end)-len(chunk))
                        if chunk == sjis_end:
                            print(_shift_jis.decode(chunk))
                            break
                    chunk = chunk[1:]
                
    def parse_fram(self, reader):
        size = reader.read_uint32()
        name = _shift_jis.decode(reader.read_bytes(size))
        print('FRAM', name)
        
        pass

    def parse_tail(self, reader):
        tail = reader.read_to_end()
        tail = tail[-25:]
        i = 0
        while i < len(tail):
            chunk = tail[i:i+4]
            match chunk:
                case b'AMBI':
                    print("AMBI", i)
                    i += 4
                    data = tail[i:i+4]
                    diffuse = dict(
                        b = data[0] / 255.0,
                        g = data[1] / 255.0,
                        r = data[2] / 255.0,
                    )
                    print(diffuse)
                    i += 4
                case b'FOGG':
                    print("FOGG", i)
                    i += 4
                    fog = dict(
                        color = struct.unpack('<I', tail[i:i+4])[0],
                        start = struct.unpack('<f', tail[i+4:i+8])[0],
                        end = struct.unpack('<f', tail[i+8:i+12])[0],
                    )
                    print(fog)
                    i += 4 * 3
                case _:
                    i += 1


def main() -> int:
    import os
    from dotenv import load_dotenv
    load_dotenv()

    appdir = os.getenv('HNH_APPDIR')
    otoko_sek = os.path.join(appdir, 'ODF/OTOKO/OTOKO.SEK')
    otoko_odf = os.path.join(appdir, 'ODF/OTOKO/OTOKO.ODF')

    with open(otoko_odf, "rb") as f:
        parser = OdfParser(f)

    return 0


if __name__ == '__main__':
    sys.exit(main())
    