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


class D3DLight():
    def __init__(self):
        self.fields = []

    def add_field(self, name, value):
        self.fields.append([name, value])

    def parse(self, reader):
        self.add_field('dltType', reader.read_uint32())
        self.add_field('dcvDiffuse', reader.read_color4())
        self.add_field('dcvSpecular', reader.read_color4())
        self.add_field('dcvAmbient', reader.read_color4())
        self.add_field('dvPosition', reader.read_vector3())
        self.add_field('dvDirection', reader.read_vector3())
        self.add_field('dvRange', reader.read_single())
        self.add_field('dvFalloff', reader.read_single())
        self.add_field('dvAttenuation0', reader.read_single())
        self.add_field('dvAttenuation1', reader.read_single())
        self.add_field('dvAttenuation2', reader.read_single())
        self.add_field('dvTheta', reader.read_single())
        self.add_field('dvPhi', reader.read_single())

from enum import Enum

ENDER = "ファイルは終了"
sjis_end, _ = _shift_jis.encode(ENDER)

class ChunkID(Enum):
    CHUNK_END = sjis_end[0:4]

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
        chunk = reader.read_bytes(4)
        fram_index = 0
        while True:
            if len(chunk) < 4:
                rest_chunk = reader.read_bytes(4-len(chunk))
                if not rest_chunk:
                    break
                chunk += rest_chunk
                
            match chunk[0:4]:
                case b'FRAM':
                    chunk = b''
                    fram_index += 1
                    self.parse_fram(reader, fram_index)
                case b'MESH':
                    chunk = b''
                    self.parse_mesh(reader)
                case b'MORP':
                    chunk = b''
                    print("MORP")
                case b'ANIM':
                    chunk = b''
                    print("ANIM")
                case b'3DON':
                    chunk = b''
                    print('3DON')
                case ChunkID.CHUNK_END:
                    chunk += reader.read_bytes(len(sjis_end)-len(chunk))
                    if chunk == sjis_end:
                        print(_shift_jis.decode(chunk))
                        break
                    chunk = chunk[1:]
                case _:
                    chunk = chunk[1:]
                
    def parse_fram(self, reader, index):
        size = reader.read_uint32()
        name, _ = _shift_jis.decode(reader.read_bytes(size))
        print('FRAM', name)

        #if name == 'CameraFrame':
            # Attach CamreFrame to this BaseFrame
        # else
            # Register Node/Frame to Array

        # Section: PARN
        peeks = reader.peek_bytes(25 + 3)
        i = 0
        peek_pos = 0
        while i < 25:
            chunk = peeks[peek_pos:peek_pos+4]
            if chunk == b'PARN':
                skipped = reader.read_bytes(peek_pos+4)
                size = reader.read_uint32()
                name = _shift_jis.decode(reader.read_bytes(size))[0]
                print('PARN', index, name)
                break  
            
            peek_pos += 1
            i += 1
        
        # Section: LIGH & FFLG
        peeks = reader.peek_bytes(25 + 3)
        i = 0
        peek_pos = 0
        while i < 25:
            fram_type = 0
            chunk = peeks[peek_pos:peek_pos+4]
            match chunk:
                case b'VX-N':
                    print(chunk)
                    fram_type = 1002
                    skipped = reader.read_bytes(peek_pos+4)
                case b'MH-N':
                    print(chunk)
                    fram_type = 1013
                    skipped = reader.read_bytes(peek_pos+4)
                case b'MP-N':
                    print(chunk)
                    fram_type = 1111
                    skipped = reader.read_bytes(peek_pos+4)
                case b'LIGH':
                    skipped = reader.read_bytes(peek_pos+4)
                    size = reader.read_uint32()
                    data = reader.read_bytes(104)
                    data_reader = BinaryReader(BytesIO(data))
                    d3dLight = D3DLight()
                    d3dLight.parse(data_reader)
                    print(chunk, size, d3dLight.fields)

                    peeks = reader.peek_bytes(25 + 3)
                    peek_pos = 0
                case b'FFLG':
                    skipped = reader.read_bytes(peek_pos+4)
                    size = reader.read_uint32()
                    flags = reader.read_uint16()
                    flag_bits = [int(bit) for bit in f"{flags:08b}"]

                    print(chunk, size, flag_bits)
                    
                    peeks = reader.peek_bytes(25 + 3)
                    peek_pos = 0
                case b'End-':
                    if len(peeks)-peek_pos < 13:
                        peeks = reader.peek_bytes(25 + 3 + (13 - (len(peeks) - peek_pos)))
                    chunk = peeks[peek_pos:peek_pos+13]
                    if chunk == b'End-Objection':
                        print(chunk)
                        skipped = reader.read_bytes(peek_pos+13)
                        break
                case _:
                    peek_pos += 1
            i += 1
        
        # Section: FMRX
        peeks = reader.peek_bytes(25 + 3)
        i = 0
        peek_pos = 0
        while i < 25:
            chunk = peeks[peek_pos:peek_pos+4]
            if chunk == b'FMRX':
                skipped = reader.read_bytes(peek_pos+4)
                size = reader.read_uint32()
                matrix = reader.read_matrix()

                print(chunk, size, matrix)
                break  
            
            peek_pos += 1
            i += 1

    def parse_d3dmaterial7_argb(self, reader):
        ca,cr,cg,cb = reader.read_color4()
        print("ambient (%0.2f, %0.2f, %0.2f, %0.2f)" % (cr,cg,cb,ca))
        ca,cr,cg,cb = reader.read_color4()
        print("diffuse (%0.2f, %0.2f, %0.2f, %0.2f)" % (cr,cg,cb,ca))   
        ca,cr,cg,cb = reader.read_color4()
        print("emissive (%0.2f, %0.2f, %0.2f, %0.2f)" % (cr,cg,cb,ca))
        ca,cr,cg,cb = reader.read_color4()
        print("specular (%0.2f, %0.2f, %0.2f, %0.2f)" % (cr,cg,cb,ca))
        cp = reader.read_single()
        print("power (%0.2f)" % (cp,))

    def parse_mesh(self, reader):
        hdr_size = reader.read_uint32()
        vert_count = reader.read_uint32()
        
        name, _ = _shift_jis.decode(reader.read_bytes(hdr_size-8))
        print('MESH', vert_count, name)

        vert_index = 0
        while vert_index < vert_count:
            chunk = reader.peek_bytes(4)
            if not chunk:
                break
            match chunk:
                case b'VERT':
                    vert_index += 1
                    chunk = reader.read_bytes(4)
                   
                    len = reader.read_uint32()
                    name = _shift_jis.decode(reader.read_bytes(len))[0]
                    print("chunk=%s vert_index=%d name=%s" % (chunk, vert_index, name))

                    count_32 = reader.read_uint32()
                    k = reader.read_bytes(32 * count_32)

                    count_2 = reader.read_uint32()
                    kk = reader.read_bytes(2 * count_2)

                    print(f"count_32=%d count2=%d" % (count_32, count_2))
                    continue
                case b'MATE':
                    chunk = reader.read_bytes(4)
                    n = reader.read_uint32()
                    name = _shift_jis.decode(reader.read_string())[0]
                    print("chunk=VERT:%s n=%d name=%s" % (chunk, n, name))

                    self.parse_d3dmaterial7_argb(reader)
                    continue
                case b'MANM':
                    print("TODO: parse VERT:MATE:MANM")
                    chunk = reader.read_bytes(4)
                    count = reader.read_uint32()

                    for i in range(count):
                        fnum = reader.read_single()

                        self.parse_d3dmaterial7_argb(reader)
                    continue
                case b'TEXT':
                    chunk = reader.read_bytes(4)
                    n = reader.read_uint32()

                    image_bmp =  _shift_jis.decode(reader.read_string())[0]
                    print("chunk=VERT:%s n=%d image_bmp=%s" % (chunk, n, image_bmp))
                    continue
                case b'VFLG':
                    chunk = reader.read_bytes(4)
                    size = reader.read_uint32()
                    flag = reader.read_uint16()

                    reader.read_bytes(size - 2)

                    subchunk = reader.peek_bytes(4)
                    if subchunk == b'BIAS':
                        subchunk = reader.read_bytes(4)
                        bias = reader.read_uint16()
                    else:
                        bias = 0

                    subchunk = reader.peek_bytes(4)
                    if subchunk == b'ADRS':
                        subchunk = reader.read_bytes(4)
                        adrs1 = reader.read_byte()
                        adrs2 = reader.read_byte()
                    else:
                        adrs1 = 0
                        adrs2 = 0

                    subchunk = reader.peek_bytes(4)
                    if subchunk == b'ALPR':
                        subchunk = reader.read_bytes(4)
                        alpr = reader.read_uint32()
                    else:
                        alpr = 0

                    print("chunk=VERT:VFLG:%s flag=%s" % (chunk, flag))
                    print("bias=%d adrs=%d,%d alpr=%d" % (bias, adrs1, adrs2, alpr))
                    continue
                case b'Mkey':
                    break
                case b'MPPL':
                    break
                
                case b'ANIM':
                    break
                case b'FRAM':
                    break
                case b'MESH':
                    break
                case b'MORP':
                    break
                case ChunkID.CHUNK_END:
                    chunk = reader.peek_bytes(len(sjis_end))
                    if chunk == sjis_end:
                        print(_shift_jis.decode(chunk))
                        break
            discard = reader.read_byte()
        
                    
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
    