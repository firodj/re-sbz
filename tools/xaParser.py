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


def _encrypt_name(name: str) -> bytes:
    if len(name) < 1:
        return b''
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

    def read_byte(self) -> int:
        return struct.unpack('<B', self._stream.read(1))[0]

    def read_bytes(self, count: int) -> bytes:
        return self._stream.read(count)

    def read_int32(self) -> int:
        return struct.unpack('<i', self._stream.read(4))[0]

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


class BinaryWriter:
    def __init__(self, stream):
        self._stream = stream

    def write_byte(self, value: int):
        self._stream.write(struct.pack('<B', value))

    def write_bytes(self, data: bytes):
        self._stream.write(data)

    def write_int32(self, value: int):
        self._stream.write(struct.pack('<i', value))

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


# --- Data Classes ---


@dataclass
class xaMaterialColor:
    Diffuse: tuple = (0.0, 0.0, 0.0, 0.0)
    Ambient: tuple = (0.0, 0.0, 0.0, 0.0)
    Specular: tuple = (0.0, 0.0, 0.0, 0.0)
    Emissive: tuple = (0.0, 0.0, 0.0, 0.0)
    Power: float = 0.0
    Unknown1: bytes = b''

    def write_to(self, writer: BinaryWriter):
        writer.write_color4(self.Diffuse)
        writer.write_color4(self.Ambient)
        writer.write_color4(self.Specular)
        writer.write_color4(self.Emissive)
        writer.write_single(self.Power)
        writer.write_bytes(self.Unknown1)


@dataclass
class xaMaterial:
    Name: str = ''
    ColorList: List[xaMaterialColor] = field(default_factory=list)

    def write_to(self, writer: BinaryWriter):
        writer.write_name(self.Name)
        writer.write_int32(len(self.ColorList))
        for color in self.ColorList:
            color.write_to(writer)


@dataclass
class xaMaterialSection:
    MaterialList: List[xaMaterial] = field(default_factory=list)

    def write_to(self, writer: BinaryWriter):
        writer.write_int32(len(self.MaterialList))
        for mat in self.MaterialList:
            mat.write_to(writer)


@dataclass
class xaSection2ItemBlock:
    Unknown1: bytes = b''
    Unknown2: bytes = b''

    def write_to(self, writer: BinaryWriter):
        writer.write_bytes(self.Unknown1)
        writer.write_bytes(self.Unknown2)


@dataclass
class xaSection2Item:
    Unknown1: bytes = b''
    Name: str = ''
    Unknown2: bytes = b''
    Unknown3: bytes = b''
    Unknown4: bytes = b''
    ItemBlockList: List[xaSection2ItemBlock] = field(default_factory=list)
    Unknown5: bytes = b''

    def write_to(self, writer: BinaryWriter):
        writer.write_bytes(self.Unknown1)
        writer.write_name(self.Name)
        writer.write_bytes(self.Unknown2)
        writer.write_bytes(self.Unknown3)
        writer.write_int32(len(self.ItemBlockList))
        writer.write_bytes(self.Unknown4)
        for block in self.ItemBlockList:
            block.write_to(writer)
        writer.write_bytes(self.Unknown5)


@dataclass
class xaSection2:
    ItemList: List[xaSection2Item] = field(default_factory=list)

    def write_to(self, writer: BinaryWriter):
        writer.write_int32(len(self.ItemList))
        for item in self.ItemList:
            item.write_to(writer)


@dataclass
class xaMorphIndexSet:
    Unknown1: bytes = b''
    MeshIndices: list = field(default_factory=list)
    MorphIndices: list = field(default_factory=list)
    Name: str = ''

    def write_to(self, writer: BinaryWriter):
        writer.write_bytes(self.Unknown1)
        writer.write_int32(len(self.MeshIndices))
        for idx in self.MeshIndices:
            writer.write_uint16(idx)
        for idx in self.MorphIndices:
            writer.write_uint16(idx)
        writer.write_name(self.Name)


@dataclass
class xaMorphKeyframe:
    PositionList: list = field(default_factory=list)
    NormalList: list = field(default_factory=list)
    Name: str = ''

    def write_to(self, writer: BinaryWriter):
        writer.write_int32(len(self.PositionList))
        for pos in self.PositionList:
            writer.write_vector3(pos)
        for normal in self.NormalList:
            writer.write_vector3(normal)
        writer.write_name(self.Name)


@dataclass
class xaMorphKeyframeRef:
    Unknown1: bytes = b''
    Index: int = 0
    Unknown2: bytes = b''
    Name: str = ''

    def write_to(self, writer: BinaryWriter):
        writer.write_bytes(self.Unknown1)
        writer.write_int32(self.Index)
        writer.write_bytes(self.Unknown2)
        writer.write_name(self.Name)


@dataclass
class xaMorphClip:
    MeshName: str = ''
    Name: str = ''
    KeyframeRefList: List[xaMorphKeyframeRef] = field(default_factory=list)
    Unknown1: bytes = b''

    def write_to(self, writer: BinaryWriter):
        writer.write_name(self.MeshName)
        writer.write_name(self.Name)
        writer.write_int32(len(self.KeyframeRefList))
        for ref in self.KeyframeRefList:
            ref.write_to(writer)
        writer.write_bytes(self.Unknown1)


@dataclass
class xaMorphSection:
    IndexSetList: List[xaMorphIndexSet] = field(default_factory=list)
    KeyframeList: List[xaMorphKeyframe] = field(default_factory=list)
    ClipList: List[xaMorphClip] = field(default_factory=list)

    def write_to(self, writer: BinaryWriter):
        writer.write_int32(len(self.IndexSetList))
        for index_set in self.IndexSetList:
            index_set.write_to(writer)
        writer.write_int32(len(self.KeyframeList))
        for keyframe in self.KeyframeList:
            keyframe.write_to(writer)
        writer.write_int32(len(self.ClipList))
        for clip in self.ClipList:
            clip.write_to(writer)


@dataclass
class xaSection4Item:
    Unknown1: bytes = b''
    Unknown2: bytes = b''
    Unknown3: bytes = b''

    def write_to(self, writer: BinaryWriter):
        writer.write_bytes(self.Unknown1)
        writer.write_bytes(self.Unknown2)
        writer.write_bytes(self.Unknown3)


@dataclass
class xaSection4:
    ItemListList: List[List[xaSection4Item]] = field(default_factory=list)

    def write_to(self, writer: BinaryWriter):
        writer.write_int32(len(self.ItemListList))
        for item_list in self.ItemListList:
            writer.write_int32(len(item_list))
            for item in item_list:
                item.write_to(writer)


@dataclass
class xaAnimationKeyframe:
    Index: int = 0
    Rotation: tuple = (0.0, 0.0, 0.0, 0.0)
    Unknown1: bytes = b''
    Translation: tuple = (0.0, 0.0, 0.0)
    Scaling: tuple = (0.0, 0.0, 0.0)

    def write_to(self, writer: BinaryWriter):
        writer.write_int32(self.Index)
        writer.write_quaternion(self.Rotation)
        writer.write_bytes(self.Unknown1)
        writer.write_vector3(self.Translation)
        writer.write_vector3(self.Scaling)


@dataclass
class xaAnimationTrack:
    Name: str = ''
    Unknown1: bytes = b''
    KeyframeList: List[xaAnimationKeyframe] = field(default_factory=list)

    def write_to(self, writer: BinaryWriter):
        writer.write_name(self.Name)
        writer.write_int32(len(self.KeyframeList))
        writer.write_bytes(self.Unknown1)
        for keyframe in self.KeyframeList:
            keyframe.write_to(writer)


@dataclass
class xaAnimationClip:
    Name: str = ''
    Speed: float = 0.0
    Unknown1: bytes = b''
    Start: float = 0.0
    End: float = 0.0
    Unknown2: bytes = b''
    Unknown3: bytes = b''
    Unknown4: bytes = b''
    Next: int = 0
    Unknown5: bytes = b''
    Unknown6: bytes = b''
    Unknown7: bytes = b''

    def write_to(self, writer: BinaryWriter):
        writer.write_name_without_length(self.Name, 64)
        writer.write_single(self.Speed)
        writer.write_bytes(self.Unknown1)
        writer.write_single(self.Start)
        writer.write_single(self.End)
        writer.write_bytes(self.Unknown2)
        writer.write_bytes(self.Unknown3)
        writer.write_bytes(self.Unknown4)
        writer.write_int32(self.Next)
        writer.write_bytes(self.Unknown5)
        writer.write_bytes(self.Unknown6)
        writer.write_bytes(self.Unknown7)


@dataclass
class xaAnimationSection:
    ClipList: List[xaAnimationClip] = field(default_factory=list)
    TrackList: List[xaAnimationTrack] = field(default_factory=list)

    def write_to(self, writer: BinaryWriter):
        for clip in self.ClipList:
            clip.write_to(writer)
        writer.write_int32(len(self.TrackList))
        for track in self.TrackList:
            track.write_to(writer)


# --- Main Parser ---


class xaParser:
    def __init__(self, stream, name=None):
        self.Header: Optional[bytes] = None
        self.MaterialSection: Optional[xaMaterialSection] = None
        self.Section2: Optional[xaSection2] = None
        self.MorphSection: Optional[xaMorphSection] = None
        self.Section4: Optional[xaSection4] = None
        self.AnimationSection: Optional[xaAnimationSection] = None
        self.Footer: bytes = b''
        self.Format: int = 0
        self.Name: Optional[str] = name

        reader = BinaryReader(stream)

        type_byte = reader.read_byte()
        if type_byte == 0x00:
            self.Format = -1
            self.Section2 = self._parse_section2(reader)
            self.MorphSection = self._parse_morph_section(reader)
            self.AnimationSection = self._parse_animation_section(reader)
        elif type_byte in (0x02, 0x03):
            self.Header = bytes([type_byte]) + reader.read_bytes(4)
            self._parse_sb3_format(reader)
        elif type_byte == 0x01:
            test_buf = reader.read_bytes(4)
            if (test_buf[0] | test_buf[1] | test_buf[2]) == 0:
                self.Header = bytes([type_byte]) + test_buf
                self._parse_sb3_format(reader)
            else:
                self.Format = -1
                num_materials = struct.unpack('<i', test_buf[0:4])[0]
                self.MaterialSection = self._parse_material_section_with_count(reader, num_materials)
                self.Section2 = self._parse_section2(reader)
                self.MorphSection = self._parse_morph_section(reader)
                self.AnimationSection = self._parse_animation_section(reader)
        else:
            raise Exception('Unable to determine .xa format')

        self.Footer = reader.read_to_end()

    def write_to(self, stream):
        writer = BinaryWriter(stream)
        if self.Header is not None:
            writer.write_bytes(self.Header)

        self._write_section(writer, self.MaterialSection)
        self._write_section(writer, self.Section2)
        self._write_section(writer, self.MorphSection)
        if self.Format >= 0x02:
            self._write_section(writer, self.Section4)
        self._write_section(writer, self.AnimationSection)

        writer.write_bytes(self.Footer)

    @staticmethod
    def _write_section(writer: BinaryWriter, section):
        if section is None:
            writer.write_byte(0)
        else:
            writer.write_byte(1)
            section.write_to(writer)

    def _parse_sb3_format(self, reader: BinaryReader):
        self.Format = struct.unpack('<i', self.Header[0:4])[0]
        self.MaterialSection = self._parse_material_section(reader)
        self.Section2 = self._parse_section2(reader)
        self.MorphSection = self._parse_morph_section(reader)
        if self.Format >= 0x02:
            self.Section4 = self._parse_section4(reader)
        self.AnimationSection = self._parse_animation_section(reader)

    @staticmethod
    def _parse_material_section(reader: BinaryReader) -> Optional[xaMaterialSection]:
        if reader.read_byte() == 1:
            return xaParser._parse_material_section_with_count(reader, reader.read_int32())
        return None

    @staticmethod
    def _parse_material_section_with_count(reader: BinaryReader, num_materials: int) -> xaMaterialSection:
        section = xaMaterialSection()
        for _ in range(num_materials):
            mat = xaMaterial()
            section.MaterialList.append(mat)
            mat.Name = reader.read_name()

            num_colors = reader.read_int32()
            for _ in range(num_colors):
                color = xaMaterialColor()
                mat.ColorList.append(color)
                color.Diffuse = reader.read_color4()
                color.Ambient = reader.read_color4()
                color.Specular = reader.read_color4()
                color.Emissive = reader.read_color4()
                color.Power = reader.read_single()
                color.Unknown1 = reader.read_bytes(4)
        return section

    @staticmethod
    def _parse_section2(reader: BinaryReader) -> Optional[xaSection2]:
        if reader.read_byte() == 0:
            return None

        section = xaSection2()
        num_items = reader.read_int32()
        for _ in range(num_items):
            item = xaSection2Item()
            section.ItemList.append(item)

            item.Unknown1 = reader.read_bytes(4)
            item.Name = reader.read_name()
            item.Unknown2 = reader.read_bytes(4)
            item.Unknown3 = reader.read_bytes(4)

            num_item_blocks = reader.read_int32()
            item.Unknown4 = reader.read_bytes(4)
            for _ in range(num_item_blocks):
                item_block = xaSection2ItemBlock()
                item.ItemBlockList.append(item_block)
                item_block.Unknown1 = reader.read_bytes(1)
                item_block.Unknown2 = reader.read_bytes(4)

            item.Unknown5 = reader.read_bytes(1)

        return section

    @staticmethod
    def _parse_morph_section(reader: BinaryReader) -> Optional[xaMorphSection]:
        if reader.read_byte() == 0:
            return None

        section = xaMorphSection()

        num_index_sets = reader.read_int32()
        for _ in range(num_index_sets):
            index_set = xaMorphIndexSet()
            section.IndexSetList.append(index_set)

            index_set.Unknown1 = reader.read_bytes(1)
            num_vertices = reader.read_int32()
            index_set.MeshIndices = reader.read_uint16_array(num_vertices)
            index_set.MorphIndices = reader.read_uint16_array(num_vertices)
            index_set.Name = reader.read_name()

        num_keyframes = reader.read_int32()
        for _ in range(num_keyframes):
            keyframe = xaMorphKeyframe()
            section.KeyframeList.append(keyframe)

            num_vertices = reader.read_int32()
            for _ in range(num_vertices):
                keyframe.PositionList.append(reader.read_vector3())
            for _ in range(num_vertices):
                keyframe.NormalList.append(reader.read_vector3())

            keyframe.Name = reader.read_name()

        num_clips = reader.read_int32()
        for _ in range(num_clips):
            clip = xaMorphClip()
            section.ClipList.append(clip)

            clip.MeshName = reader.read_name()
            clip.Name = reader.read_name()

            num_keyframe_refs = reader.read_int32()
            for _ in range(num_keyframe_refs):
                keyframe_ref = xaMorphKeyframeRef()
                clip.KeyframeRefList.append(keyframe_ref)

                keyframe_ref.Unknown1 = reader.read_bytes(1)
                keyframe_ref.Index = reader.read_int32()
                keyframe_ref.Unknown2 = reader.read_bytes(1)
                keyframe_ref.Name = reader.read_name()

            clip.Unknown1 = reader.read_bytes(4)

        return section

    @staticmethod
    def _parse_section4(reader: BinaryReader) -> Optional[xaSection4]:
        if reader.read_byte() == 0:
            return None

        section = xaSection4()
        num_item_lists = reader.read_int32()
        for _ in range(num_item_lists):
            num_items = reader.read_int32()
            item_list = []
            section.ItemListList.append(item_list)
            for _ in range(num_items):
                item = xaSection4Item()
                item.Unknown1 = reader.read_bytes(104)
                item.Unknown2 = reader.read_bytes(4)
                item.Unknown3 = reader.read_bytes(64)
                item_list.append(item)

        return section

    def _parse_animation_section(self, reader: BinaryReader) -> Optional[xaAnimationSection]:
        if reader.read_byte() == 0:
            return None

        section = xaAnimationSection()

        if self.Format == 0x03:
            num_clips = 1024
        else:
            num_clips = 512

        for _ in range(num_clips):
            clip = xaAnimationClip()
            section.ClipList.append(clip)

            clip.Name = reader.read_name(64)
            clip.Speed = reader.read_single()
            clip.Unknown1 = reader.read_bytes(4)
            clip.Start = reader.read_single()
            clip.End = reader.read_single()
            clip.Unknown2 = reader.read_bytes(1)
            clip.Unknown3 = reader.read_bytes(1)
            clip.Unknown4 = reader.read_bytes(1)
            clip.Next = reader.read_int32()
            clip.Unknown5 = reader.read_bytes(1)
            clip.Unknown6 = reader.read_bytes(4)
            clip.Unknown7 = reader.read_bytes(16)

        num_tracks = reader.read_int32()
        for _ in range(num_tracks):
            track = xaAnimationTrack()
            section.TrackList.append(track)

            track.Name = reader.read_name()
            num_keyframes = reader.read_int32()
            track.Unknown1 = reader.read_bytes(4)

            for _ in range(num_keyframes):
                keyframe = xaAnimationKeyframe()
                track.KeyframeList.append(keyframe)

                keyframe.Index = reader.read_int32()
                keyframe.Rotation = reader.read_quaternion()
                keyframe.Unknown1 = reader.read_bytes(8)
                keyframe.Translation = reader.read_vector3()
                keyframe.Scaling = reader.read_vector3()

        return section

# Example usage:
if __name__ == "__main__":
    import os
    from dotenv import load_dotenv
    load_dotenv()

    appdir = os.getenv('APPDIR')
    xafilename = 'out/sb03_00/hand_body_L_00.xa'
    print("File:", xafilename)
    # Example of how to use the parser
    with open(os.path.join(appdir, xafilename), "rb") as f:
        parser = xaParser(f, "hand_body_L_00.xa")
        print(f"Format: {parser.Format}")
        #print(f"MaterialSection: {parser.MaterialSection}")
        #print(f"MorphSection: {parser.MorphSection}")
        #print(f"AnimationSection: {parser.AnimationSection}")
    pass
