import struct
from io import BytesIO
from dataclasses import dataclass, field
from typing import List, Optional
from binaryhelper import BinaryReader, BinaryWriter, red_print
import sys
from enum import Enum

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

    def __str__(self):
        return "D3DLight(%s)" % (self.fields)

def argb_to_rgba(v):
    a,r,g,b = v
    return (r,g,b,a)

class D3DMaterial7():
    def __init__(self):
        self.fields = []

    def add_field(self, name, value):
        self.fields.append([name, value])

    def parse_argb(self, reader):
        self.add_field('dcvDiffuse',  argb_to_rgba(reader.read_color4()))
        self.add_field('dcvAmbient',  argb_to_rgba(reader.read_color4()))
        self.add_field('dcvSpecular', argb_to_rgba(reader.read_color4()))
        self.add_field('dcvEmissive', argb_to_rgba(reader.read_color4()))
        self.add_field('dvPower', reader.read_single())
    
    def __str__(self):
        return "D3DMaterial7(%s)" % self.fields

class D3DVertex():
    def __init__(self):
        self.fields = []

    def add_field(self, name, value):
        self.fields.append([name, value])
        
    def __str__(self):
        return "D3DVertex(%s)" % self.fields

    def parse(self, reader):
        self.add_field("dv", reader.read_vector3())
        self.add_field("dvN", reader.read_vector3())
        self.add_field("dvT", reader.read_vector2())

class HanaAnimKey():
    def __init__(self):
        self.fields = []

    def add_field(self, name, value):
        self.fields.append([name, value])

    def __str__(self):
        return "HanaAnimKey(%s)" % self.fields

    def parse(self, reader):
        self.add_field("t", reader.read_single())
        self.add_field("pos", reader.read_vector3())
        self.add_field("scale", reader.read_vector3())
        self.add_field("quart", reader.read_quaternion())
        self.add_field("gap", reader.read_uint32_array(3))

class HanaAnim():
    def __init__(self):
        self.count = 0
        self.keys = []
        self.name = ''

    def parse(self, reader):
        n = reader.read_uint32()
        self.count = n // 56
        print('ANIM count=%d name=%s' % (self.count, self.name), ":")
        for i in range(self.count):
            key = HanaAnimKey()
            key.parse(reader)
            self.keys.append(key)
            print(" -", i, key)

        print('=== END-ANIM (%s)' % (self.name))

class Hana3DonItem():
    def __init__(self):
        self.fields = []

    def add_field(self, name, value):
        self.fields.append([name, value])

    def __str__(self):
        return "Hana3DonItem(%s)" % self.fields
        
    def parse(self, reader):
        self.add_field('a', reader.read_single())
        self.add_field('b', reader.read_single())
        self.add_field('c', reader.read_uint32())
        self.add_field('name1', reader.read_jpstring())
        self.add_field('name2', reader.read_jpstring())

class Hana3Don():
    def __init__(self):
        self.count = 0
        self.items = []

    def parse(self, reader):
        self.count = reader.read_uint32()

        print("3DON count=%d" % (self.count,))
        for i in range(self.count):
            item = Hana3DonItem()
            item.parse(reader)
            self.items.append(item)

            print(i, item)

JP_SOUND = "サウンド"
sjis_sound, _ = _shift_jis.encode(JP_SOUND)

JP_ENDOFFILE = "ファイルは終了"
sjis_endoffile, _ = _shift_jis.encode(JP_ENDOFFILE)

asc_endobjection = b'End-Objection'
asc_light = b'Light'

class ChunkID(Enum):
    CHUNK_ENDOFFILE = sjis_endoffile[0:4]
    CHUNK_SOUND = sjis_sound[0:4]
    CHUNK_FRAM = b'FRAM'
    CHUNK_MESH = b'MESH'
    CHUNK_MORP = b'MORP'
    CHUNK_ANIM = b'ANIM'
    CHUNK_3DON = b'3DON'
    CHUNK_PARN = b'PARN'
    CHUNK_VX_N = b'VX-N'
    CHUNK_MH_N = b'MH-N'
    CHUNK_MP_N = b'MP-N'
    CHUNK_LIGH = b'LIGH'
    CHUNK_FFLG = b'FFLG'
    CHUNK_ENDOBJECTION = b'End-'
    CHUNK_FMRX = b'FMRX'
    CHUNK_VERT = b'VERT'
    CHUNK_MATE = b'MATE'
    CHUNK_MANM = b'MANM'
    CHUNK_TEXT = b'TEXT'
    CHUNK_VFLG = b'VFLG'
    CHUNK_MKEY = b'Mkey'
    CHUNK_MPPL = b'MPPL'
    CHUNK_BIAS = b'BIAS'
    CHUNK_ADRS = b'ADRS'
    CHUNK_ALPR = b'ALPR'
    CHUNK_AMBI = b'AMBI'
    CHUNK_FOGG = b'FOGG'
    CHUNK_SIZE = b'SIZE'
    CHUNK_NAME = b'NAME'
    CHUNK_SEKN = b'SEKN'
    CHUNK_EYES = b'EYES'
    CHUNK_EYE2 = b'EYE2'
    CHUNK_LIGHT = b'Ligh'

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
        current_fram_name = None
        current_fram_index = None
        while True:
            if len(chunk) < 4:
                rest_chunk = reader.read_bytes(4-len(chunk))
                if not rest_chunk:
                    break
                chunk += rest_chunk
                
            match chunk[0:4]:
                case ChunkID.CHUNK_FRAM.value:
                    chunk = b''
                    fram_index += 1
                    current_fram_index, current_fram_name = self.parse_fram(reader, fram_index)
                case ChunkID.CHUNK_MESH.value:
                    chunk = b''
                    self.parse_mesh(reader)
                case ChunkID.CHUNK_MORP.value:
                    chunk = b''
                    raise NotImplementedError("MORP feature is not implemented yet.")
                case ChunkID.CHUNK_ANIM.value:
                    chunk = b''
                    anim = HanaAnim()
                    if current_fram_name is not None:
                        anim.name = "%s_Animation" % (current_fram_name,)
                    anim.parse(reader)
                    
                case ChunkID.CHUNK_3DON.value:
                    chunk = b''
                    hana3don = Hana3Don()
                    hana3don.parse(reader)
                case ChunkID.CHUNK_ENDOFFILE.value:
                    chunk += reader.read_bytes(len(sjis_endoffile)-len(chunk))
                    if chunk == sjis_endoffile:
                        print(_shift_jis.decode(chunk))
                        break
                    chunk = chunk[1:]
                case _:
                    chunk = chunk[1:]
                
    def parse_fram(self, reader, index):
        size = reader.read_uint32()
        name = reader.read_jpbytes(size) # _shift_jis.decode(reader.read_bytes(size))
        print('FRAM', index, name)

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
            if chunk == ChunkID.CHUNK_PARN.value:
                skipped = reader.read_bytes(peek_pos+4)
                size = reader.read_uint32()
                parent_name = reader.read_jpbytes(size) # _shift_jis.decode(reader.read_bytes(size))[0]
                print('PAReNt name=%s' % (parent_name,))
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
                case ChunkID.CHUNK_VX_N.value:
                    print(chunk)
                    fram_type = 1002
                    skipped = reader.read_bytes(peek_pos+4)
                case ChunkID.CHUNK_MH_N.value:
                    print(chunk)
                    fram_type = 1013
                    skipped = reader.read_bytes(peek_pos+4)
                case ChunkID.CHUNK_MP_N.value:
                    print(chunk)
                    fram_type = 1111
                    skipped = reader.read_bytes(peek_pos+4)
                case ChunkID.CHUNK_LIGH.value:
                    skipped = reader.read_bytes(peek_pos+4)
                    size = reader.read_uint32()
                    data = reader.read_bytes(104)
                    data_reader = BinaryReader(BytesIO(data))
                    d3dLight = D3DLight()
                    d3dLight.parse(data_reader)
                    print(chunk.decode(), size, d3dLight)

                    peeks = reader.peek_bytes(25 + 3)
                    peek_pos = 0
                case ChunkID.CHUNK_FFLG.value:
                    skipped = reader.read_bytes(peek_pos+4)
                    size = reader.read_uint32()
                    flags = reader.read_uint16()
                    flag_bits = [int(bit) for bit in f"{flags:08b}"]

                    print(chunk.decode(), size, flag_bits)
                    
                    peeks = reader.peek_bytes(25 + 3)
                    peek_pos = 0
                case ChunkID.CHUNK_ENDOBJECTION.value:
                    if len(peeks)-peek_pos < 13:
                        peeks = reader.peek_bytes(25 + 3 + (13 - (len(peeks) - peek_pos)))
                    chunk = peeks[peek_pos:peek_pos+13]
                    if chunk == asc_endobjection:
                        print(chunk.decode())
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
            if chunk == ChunkID.CHUNK_FMRX.value:
                skipped = reader.read_bytes(peek_pos+4)
                size = reader.read_uint32()
                matrix = reader.read_matrix()

                print(chunk.decode(), size, matrix)
                break  
            
            peek_pos += 1
            i += 1
        print('=== END-FRAM', index, name)
        return (index, name)

    def parse_mesh(self, reader):
        hdr_size = reader.read_uint32()
        vert_count = reader.read_uint32()
        
        size = hdr_size-8
        mesh_name = reader.read_jpbytes(size) #_shift_jis.decode(reader.read_bytes(hdr_size-8))
        print('MESH vert_count=%d name=%s' % (vert_count, mesh_name))

        vert_index = 0
        while True:
            chunk = reader.peek_bytes(4)
            if not chunk:
                break
            match chunk:
                case ChunkID.CHUNK_VERT.value:
                    vert_index += 1
                    chunk = reader.read_bytes(4)
                   
                    len = reader.read_uint32()
                    vert_name = reader.read_jpbytes(len) # _shift_jis.decode(reader.read_bytes(len))[0]
                    print("-- %d -- VERT name=%s" % (vert_index, vert_name))

                    sizeof_d3dvertex = 32
                    count_32 = reader.read_uint32()
                    #k = reader.read_bytes(32 * count_32)
                    print("vertex_count =", count_32, ":")
                    for k in range(count_32):
                        vertex = D3DVertex()
                        vertex.parse(reader)
                        print(" -", k, vertex)

                    count_2 = reader.read_uint32()
                    #kk = reader.read_bytes(2 * count_2)
                    print("indices_count =", count_2, ":")
                    faces = None
                    for kk in range(count_2):
                        if kk % 3 == 0:
                            if faces is not None:
                                print(" -", kk // 3, faces)
                            faces = [0, 0, 0]
                        w = reader.read_uint16()
                        faces[kk % 3] = w
                    print(" -", count_2 // 3, faces)

                    print(f"name2=%d_%s_???" % (vert_index, mesh_name))
                    continue
                case ChunkID.CHUNK_MATE.value:
                    chunk = reader.read_bytes(4)
                    n = reader.read_uint32()
                    name = reader.read_jpstring()
                    print("chunk=VERT:MATE n=%d name=%s" % (n, name))

                    material = D3DMaterial7()
                    material.parse_argb(reader)
                    print(material)
             
                    continue
                case ChunkID.CHUNK_MANM.value:
                    print("TODO: parse VERT:MATE:MANM")
                    chunk = reader.read_bytes(4)
                    count = reader.read_uint32()

                    for i in range(count):
                        fnum = reader.read_single()

                        material = D3DMaterial7()
                        material.parse_argb(reader)
                        print(material)
                    continue
                case ChunkID.CHUNK_TEXT.value:
                    chunk = reader.read_bytes(4)
                    n = reader.read_uint32()

                    image_bmp =  reader.read_jpstring()
                    print("chunk=VERT:TEXTure n=%d image_bmp=%s" % (n, image_bmp))
                    continue
                case ChunkID.CHUNK_VFLG.value:
                    chunk = reader.read_bytes(4)
                    size = reader.read_uint32()
                    flag = reader.read_uint16()

                    reader.read_bytes(size - 2)

                    subchunk = reader.peek_bytes(4)
                    if subchunk == ChunkID.CHUNK_BIAS.value:
                        subchunk = reader.read_bytes(4)
                        bias = reader.read_uint16()
                    else:
                        bias = 0

                    subchunk = reader.peek_bytes(4)
                    if subchunk == ChunkID.CHUNK_ADRS.value:
                        subchunk = reader.read_bytes(4)
                        adrs1 = reader.read_byte()
                        adrs2 = reader.read_byte()
                    else:
                        adrs1 = 0
                        adrs2 = 0

                    subchunk = reader.peek_bytes(4)
                    if subchunk == ChunkID.CHUNK_ALPR.value:
                        subchunk = reader.read_bytes(4)
                        alpr = reader.read_uint32()
                    else:
                        alpr = 0

                    print("chunk=VERT:VFLG flag=%s bias=%d adrs=%d,%d alpr=%d" % (flag, bias, adrs1, adrs2, alpr))

                    continue
                case ChunkID.CHUNK_MKEY.value:
                    break
                case ChunkID.CHUNK_MPPL.value:
                    break
                case ChunkID.CHUNK_ANIM.value:
                    break
                case ChunkID.CHUNK_FRAM.value:
                    break
                case ChunkID.CHUNK_MESH.value:
                    break
                case ChunkID.CHUNK_MORP.value:
                    break
                case ChunkID.CHUNK_ENDOFFILE.value:
                    chunk = reader.peek_bytes(len(sjis_endoffile))
                    if chunk == sjis_endoffile:
                        chunk = reader.read_bytes(len(chunk))
                        print(_shift_jis.decode(chunk))
                        break
           
            discard = reader.read_byte()
       
        print('=== END-MESH (%s)' % (mesh_name,))
  
    def parse_tail(self, reader):
        tail = reader.read_to_end()
        tail = tail[-25:]
        i = 0
        while i < len(tail):
            chunk = tail[i:i+4]
            match chunk:
                case ChunkID.CHUNK_AMBI.value:
                    print(chunk.decode(), i)
                    i += 4
                    data = tail[i:i+4]
                    diffuse = dict(
                        b = data[0] / 255.0,
                        g = data[1] / 255.0,
                        r = data[2] / 255.0,
                    )
                    print(diffuse)
                    i += 4
                case ChunkID.CHUNK_FOGG.value:
                    print(chunk.decode(), i)
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

class OdsFile():
    def __init__(self, stream):
        reader = BinaryReader(stream)
        self.parse(reader)

    def parse(self, reader):
        header = "新フォーマットＯＤＳ%c%c%c" % (13, 10, 0)
        sjis, _ = _shift_jis.encode(header)
        hdr = reader.read_bytes(len(sjis))
        
        if hdr == sjis:
            print(_shift_jis.decode(hdr))
            self.parse_content(reader)

    def parse_content(self, reader):
        
        while True:
            chunk = reader.peek_bytes(4)
            if not chunk:
                break
           
            match chunk:
                case ChunkID.CHUNK_TEXT.value:
                    chunk = reader.read_bytes(4)
                    n = reader.read_uint32()

                    filename = reader.read_jpstring()
                    print("chunk=%s n=%d filename=%s" % (chunk.decode(), n, filename))
                    continue
                case ChunkID.CHUNK_SIZE.value:
                    chunk = reader.read_bytes(4)
                    count = reader.read_uint32()
                    print("chunk=%s count=%d" % (chunk.decode(), count))
                    continue
                case ChunkID.CHUNK_NAME.value:
                    chunk = reader.read_bytes(4)
                    name = reader.read_jpstring()
                    print("chunk=%s name=%s" % (chunk.decode(), name))
                    continue
                case ChunkID.CHUNK_ANIM.value:
                    chunk = reader.read_bytes(4)
                    anim = HanaAnim()
                    anim.parse(reader)

                    continue
                case ChunkID.CHUNK_SEKN.value:
                    chunk = reader.read_bytes(4)
                    name = reader.read_jpstring()
                    print("chunk=%s name=%s" % (chunk.decode(), name))

                    count = reader.read_uint32()
                    for i in range(count):
                        a = reader.read_single()
                        b = reader.read_int32()
                        print("%d (%.2f,%d)" % (i, a, b))
                    continue
                case ChunkID.CHUNK_SOUND.value:
                    chunk = reader.peek_bytes(len(sjis_sound))
                    if chunk == sjis_sound:
                        chunk = reader.read_bytes(len(chunk))
                        continue
                case ChunkID.CHUNK_ENDOFFILE.value:
                    chunk = reader.peek_bytes(len(sjis_endoffile))
                    if chunk == sjis_endoffile:
                        chunk = reader.read_bytes(len(chunk))
                        print(_shift_jis.decode(chunk))
                        break
                case ChunkID.CHUNK_EYES.value:
                    chunk = reader.peek_bytes(8)
                    if chunk == b'EYESANIM':
                        chunk = reader.read_bytes(len(chunk))
                        anim = HanaAnim()
                        anim.parse(reader)
                        continue
                case ChunkID.CHUNK_EYE2.value:
                    chunk = reader.read_bytes(len(chunk))
                    print("perform eye scene")
                    continue
                case ChunkID.CHUNK_LIGHT.value:
                    chunk = reader.peek_bytes(5)
                    if chunk == asc_light:
                        data = reader.read_bytes(104)
                        data_reader = BinaryReader(BytesIO(data))
                        d3dLight = D3DLight()
                        d3dLight.parse(data_reader)
                        print(chunk.decode(), d3dLight)
                        continue
                case ChunkID.CHUNK_3DON.value:
                    chunk = reader.read_bytes(len(chunk))
                    hana3don = Hana3Don()
                    hana3don.parse(reader)
                    
                    continue

            print(chunk)
            discard = reader.read_byte()

class SekParser:
    def __init__(self, stream):
        reader = BinaryReader(stream)

        path1 = reader.read_bytes(256).strip(b'\0')
        name1 = reader.read_bytes(256).strip(b'\0')

        print(path1, name1)
        self.parse_body(reader)

    def parse_body(self, reader):
        szname_0 = reader.read_bytes(256).strip(b'\0')
        szname_100 = reader.read_bytes(256).strip(b'\0')
        field_200 = reader.read_uint32()
        gap_204 = reader.read_uint32()
        field_208 = reader.read_uint32()
        gap_20C = reader.read_uint32()
        gap_210 = reader.read_uint32_array(4)
        sekheader_220 = reader.read_uint32()
        gap_224 = reader.read_uint32_array(3)
        gap_230 = reader.read_uint32_array(4)
        gap_240 = reader.read_uint32_array(4)
        # gap_254 = reader.read_bytes(16).strip(b'\0')
        # gap_264 = reader.read_uint32_array(4)
        # gap_274 = reader.read_uint32_array(4)
        # gap_284 = reader.read_uint32_array(4)
        
        print(szname_100, field_200, field_208, hex(sekheader_220))
        # print(gap_244)
        # print(gap_254)
    
        for i in range(128):
            item_0 = reader.read_uint32()
            name_4 = reader.read_bytes(64).split(b'\0')[0]
            items_44 = reader.read_uint32_array(3)
            item_50 = reader.read_uint32()
            floats_54 = [reader.read_single(),reader.read_single(),reader.read_single(),reader.read_single()]
            fields_64 = reader.read_uint32_array(14)
            print(i, item_0, name_4, items_44,item_50, floats_54, fields_64)
        
def main() -> int:
    import os
    from dotenv import load_dotenv
    load_dotenv()

    appdir = os.getenv('HNH_APPDIR')

    match sys.argv[1]:
        case 'sek':
            otoko_sek = os.path.join(appdir, 'ODF/OTOKO/OTOKO.SEK')
            otoko_sek = os.path.join(appdir, 'ODF/H/H01/H01_02.SEK')
            with open(otoko_sek, "rb") as f:
                parser = SekParser(f)
        case 'odf':
            
            otoko_odf = os.path.join(appdir, 'ODF/OTOKO/OTOKO.ODF')

            with open(otoko_odf, "rb") as f:
                parser = OdfParser(f)
        case 'ods':
            p01_ods = os.path.join(appdir, 'ODS/P01.ODS')

            with open(p01_ods, "rb") as f:
                parser = OdsFile(f)
        
        case _:
            print("incorrect argument (odf, ods)")

    return 0


if __name__ == '__main__':
    sys.exit(main())
    