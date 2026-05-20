import struct
import io
from dataclasses import dataclass, field
from typing import List, Optional, Any, Tuple
import re


class Utility:
    """Utility class with encoding and name encryption/decryption methods."""
    
    ENCODING_SHIFT_JIS = 'shift_jis'
    
    @staticmethod
    def decrypt_name(buf: bytes) -> str:
        """Decrypt a name buffer using bitwise NOT operation."""
        if len(buf) < 1:
            return ""
        decrypted = bytes(~b & 0xFF for b in buf)
        return decrypted.decode(Utility.ENCODING_SHIFT_JIS).rstrip('\x00')
    
    @staticmethod
    def encrypt_name(name: str, length: int = -1) -> bytes:
        """Encrypt a name string using bitwise NOT operation."""
        if length == -1:
            encoded = name.encode(Utility.ENCODING_SHIFT_JIS)
            encrypted = bytes(~b & 0xFF for b in encoded)
            return encrypted
        else:
            encrypted = bytearray(length)
            name_bytes = name.encode(Utility.ENCODING_SHIFT_JIS)
            for i in range(min(len(name_bytes), length)):
                encrypted[i] = ~name_bytes[i] & 0xFF
            return bytes(encrypted)


class BinaryReaderExt:
    """Extended BinaryReader with methods for reading SB3Utility formats."""
    
    def __init__(self, stream: io.BytesIO):
        self.stream = stream
    
    def read_bytes(self, count: int) -> bytes:
        return self.stream.read(count)
    
    def read_int32(self) -> int:
        return struct.unpack('<i', self.stream.read(4))[0]
    
    def read_uint16(self) -> int:
        return struct.unpack('<H', self.stream.read(2))[0]
    
    def read_uint32(self) -> int:
        return struct.unpack('<I', self.stream.read(4))[0]
    
    def read_single(self) -> float:
        return struct.unpack('<f', self.stream.read(4))[0]
    
    def read_byte(self) -> int:
        return struct.unpack('<B', self.stream.read(1))[0]
    
    def read_name(self, length: int = -1) -> str:
        """Read an encrypted name from the stream."""
        if length == -1:
            name_len = self.read_int32()
            name_buf = self.read_bytes(name_len)
        else:
            name_buf = self.read_bytes(length)
        return Utility.decrypt_name(name_buf)
    
    def read_matrix(self) -> List[List[float]]:
        """Read a 4x4 matrix."""
        matrix = []
        for i in range(4):
            row = []
            for j in range(4):
                row.append(self.read_single())
            matrix.append(row)
        return matrix
    
    def read_vector2(self) -> Tuple[float, float]:
        """Read a Vector2."""
        x = self.read_single()
        y = self.read_single()
        return (x, y)
    
    def read_vector3(self) -> Tuple[float, float, float]:
        """Read a Vector3."""
        x = self.read_single()
        y = self.read_single()
        z = self.read_single()
        return (x, y, z)
    
    def read_color4(self) -> Tuple[float, float, float, float]:
        """Read a Color4 (RGBA)."""
        r = abs(self.read_single())
        g = abs(self.read_single())
        b = abs(self.read_single())
        a = abs(self.read_single())
        return (r, g, b, a)
    
    def read_single_array(self, length: int) -> List[float]:
        """Read an array of floats."""
        return [self.read_single() for _ in range(length)]
    
    def read_uint16_array(self, length: int) -> List[int]:
        """Read an array of uint16."""
        return [self.read_uint16() for _ in range(length)]
    
    def read_bytes_raw(self, count: int) -> bytes:
        """Read raw bytes (alias for read_bytes)."""
        return self.read_bytes(count)


class BinaryWriterExt:
    """Extended BinaryWriter with methods for writing SB3Utility formats."""
    
    def __init__(self, stream: io.BytesIO):
        self.stream = stream
    
    def write(self, data):
        """Write data to the stream."""
        if isinstance(data, bytes):
            self.stream.write(data)
        elif isinstance(data, bytearray):
            self.stream.write(bytes(data))
        elif isinstance(data, int):
            # Default to 4-byte int
            self.stream.write(struct.pack('<i', data))
        elif isinstance(data, float):
            self.stream.write(struct.pack('<f', data))
        elif isinstance(data, tuple):
            # Handle tuples as vectors/colors
            for val in data:
                self.stream.write(struct.pack('<f', val))
        elif isinstance(data, list):
            # Handle lists of floats (matrices, arrays)
            for val in data:
                if isinstance(val, list):
                    for v in val:
                        self.stream.write(struct.pack('<f', v))
                else:
                    self.stream.write(struct.pack('<f', val))
    
    def write_name(self, name: str):
        """Write an encrypted name to the stream."""
        name_buf = Utility.encrypt_name(name)
        self.write(len(name_buf))
        self.write(name_buf)
    
    def write_if_not_null(self, data: Optional[bytes]):
        """Write bytes if not None."""
        if data is not None:
            self.write(data)


@dataclass
class xxFace:
    """Represents a face with vertex indices."""
    VertexIndices: List[int] = field(default_factory=list)
    
    def write_to(self, stream: io.BytesIO):
        writer = BinaryWriterExt(stream)
        for idx in self.VertexIndices:
            stream.write(struct.pack('<H', idx))
    
    def clone(self) -> 'xxFace':
        face = xxFace()
        face.VertexIndices = self.VertexIndices.copy()
        return face


@dataclass
class xxVertex:
    """Base class for vertices."""
    Index: int = 0
    Position: Tuple[float, float, float] = (0.0, 0.0, 0.0)
    Weights3: List[float] = field(default_factory=lambda: [0.0, 0.0, 0.0])
    BoneIndices: bytes = b'\x00\x00\x00\x00'
    Normal: Tuple[float, float, float] = (0.0, 0.0, 0.0)
    UV: List[float] = field(default_factory=lambda: [0.0, 0.0])
    Unknown1: Optional[bytes] = None
    
    def weights4(self, skinned: bool) -> List[float]:
        """Convert 3 weights to 4 weights."""
        w3 = self.Weights3
        if skinned:
            w4 = [w3[0], w3[1], w3[2], 1.0 - (w3[0] + w3[1] + w3[2])]
            if w4[3] < 0:
                w4[3] = 0
            return w4
        return [0.0, 0.0, 0.0, 0.0]
    
    def write_to(self, stream: io.BytesIO):
        self._write_index(stream)
        writer = BinaryWriterExt(stream)
        writer.write(self.Position)
        for w in self.Weights3:
            stream.write(struct.pack('<f', w))
        stream.write(self.BoneIndices)
        writer.write(self.Normal)
        for uv in self.UV:
            stream.write(struct.pack('<f', uv))
        if self.Unknown1 is not None:
            stream.write(self.Unknown1)
    
    def _write_index(self, stream: io.BytesIO):
        raise NotImplementedError
    
    def clone(self) -> 'xxVertex':
        vertex = self.__class__()
        vertex.Index = self.Index
        vertex.Position = self.Position
        vertex.Weights3 = self.Weights3.copy()
        vertex.BoneIndices = self.BoneIndices
        vertex.Normal = self.Normal
        vertex.UV = self.UV.copy()
        vertex.Unknown1 = self.Unknown1
        return vertex


@dataclass
class xxVertexUShort(xxVertex):
    """Vertex with ushort index."""
    
    def _write_index(self, stream: io.BytesIO):
        stream.write(struct.pack('<H', self.Index))


@dataclass
class xxVertexInt(xxVertex):
    """Vertex with int index."""
    
    def _write_index(self, stream: io.BytesIO):
        stream.write(struct.pack('<i', self.Index))


@dataclass
class xxBone:
    """Represents a bone."""
    Name: str = ""
    Index: int = 0
    Matrix: List[List[float]] = field(default_factory=lambda: [[0.0]*4 for _ in range(4)])
    
    def write_to(self, stream: io.BytesIO):
        writer = BinaryWriterExt(stream)
        writer.write_name(self.Name)
        stream.write(struct.pack('<i', self.Index))
        for row in self.Matrix:
            for val in row:
                stream.write(struct.pack('<f', val))
    
    def clone(self) -> 'xxBone':
        bone = xxBone()
        bone.Name = self.Name
        bone.Index = self.Index
        bone.Matrix = [row.copy() for row in self.Matrix]
        return bone


@dataclass
class xxMaterialTexture:
    """Represents a material texture."""
    Name: str = ""
    Unknown1: bytes = b'\x00' * 16
    
    def write_to(self, stream: io.BytesIO):
        writer = BinaryWriterExt(stream)
        writer.write_name(self.Name)
        stream.write(self.Unknown1)
    
    def clone(self) -> 'xxMaterialTexture':
        mat_tex = xxMaterialTexture()
        mat_tex.Name = self.Name
        mat_tex.Unknown1 = bytes(self.Unknown1)
        return mat_tex


@dataclass
class xxMaterial:
    """Represents a material."""
    Name: str = ""
    Diffuse: Tuple[float, float, float, float] = (0.0, 0.0, 0.0, 0.0)
    Ambient: Tuple[float, float, float, float] = (0.0, 0.0, 0.0, 0.0)
    Specular: Tuple[float, float, float, float] = (0.0, 0.0, 0.0, 0.0)
    Emissive: Tuple[float, float, float, float] = (0.0, 0.0, 0.0, 0.0)
    Power: float = 0.0
    Textures: List[xxMaterialTexture] = field(default_factory=list)
    Unknown1: Optional[bytes] = None
    
    def __post_init__(self):
        if not self.Textures:
            self.Textures = [xxMaterialTexture() for _ in range(4)]
    
    def write_to(self, stream: io.BytesIO):
        writer = BinaryWriterExt(stream)
        writer.write_name(self.Name)
        writer.write(self.Diffuse)
        writer.write(self.Ambient)
        writer.write(self.Specular)
        writer.write(self.Emissive)
        stream.write(struct.pack('<f', self.Power))
        for tex in self.Textures:
            tex.write_to(stream)
        stream.write(self.Unknown1)
    
    def clone(self) -> 'xxMaterial':
        mat = xxMaterial()
        mat.Name = self.Name
        mat.Diffuse = self.Diffuse
        mat.Ambient = self.Ambient
        mat.Specular = self.Specular
        mat.Emissive = self.Emissive
        mat.Power = self.Power
        mat.Textures = [tex.clone() for tex in self.Textures]
        mat.Unknown1 = bytes(self.Unknown1) if self.Unknown1 else None
        return mat


@dataclass
class xxTexture:
    """Represents a texture."""
    Name: str = ""
    Unknown1: bytes = b'\x00' * 4
    Width: int = 0
    Height: int = 0
    Depth: int = 0
    MipLevels: int = 0
    Format: int = 0
    ResourceType: int = 0
    ImageFileFormat: int = 0
    Checksum: int = 0
    ImageData: bytes = b''
    
    def write_to(self, stream: io.BytesIO):
        writer = BinaryWriterExt(stream)
        writer.write_name(self.Name)
        stream.write(self.Unknown1)
        stream.write(struct.pack('<i', self.Width))
        stream.write(struct.pack('<i', self.Height))
        stream.write(struct.pack('<i', self.Depth))
        stream.write(struct.pack('<i', self.MipLevels))
        stream.write(struct.pack('<i', self.Format))
        stream.write(struct.pack('<i', self.ResourceType))
        stream.write(struct.pack('<i', self.ImageFileFormat))
        stream.write(struct.pack('<B', self.Checksum))
        stream.write(struct.pack('<i', len(self.ImageData)))
        stream.write(self.ImageData)
    
    def clone(self) -> 'xxTexture':
        tex = xxTexture()
        tex.Name = self.Name
        tex.Unknown1 = bytes(self.Unknown1)
        tex.Width = self.Width
        tex.Height = self.Height
        tex.Depth = self.Depth
        tex.MipLevels = self.MipLevels
        tex.Format = self.Format
        tex.ResourceType = self.ResourceType
        tex.ImageFileFormat = self.ImageFileFormat
        tex.Checksum = self.Checksum
        tex.ImageData = bytes(self.ImageData)
        return tex


@dataclass
class xxSubmesh:
    """Represents a submesh."""
    Unknown1: Optional[bytes] = None
    MaterialIndex: int = 0
    FaceList: List[xxFace] = field(default_factory=list)
    VertexList: List[xxVertex] = field(default_factory=list)
    Unknown2: Optional[bytes] = None
    Vector2Lists: List[List[Tuple[float, float]]] = field(default_factory=list)
    Unknown3: Optional[bytes] = None
    Unknown4: Optional[bytes] = None
    Unknown5: Optional[bytes] = None
    Unknown6: Optional[bytes] = None
    
    def write_to(self, stream: io.BytesIO):
        writer = BinaryWriterExt(stream)
        stream.write(self.Unknown1)
        stream.write(struct.pack('<i', self.MaterialIndex))
        
        stream.write(struct.pack('<i', len(self.FaceList) * 3))
        for face in self.FaceList:
            face.write_to(stream)
        
        stream.write(struct.pack('<i', len(self.VertexList)))
        for vertex in self.VertexList:
            vertex.write_to(stream)
        
        if self.Unknown2 is not None:
            stream.write(self.Unknown2)
        
        if self.Vector2Lists:
            for vector_list in self.Vector2Lists:
                for vec in vector_list:
                    writer.write(vec)
        
        if self.Unknown3 is not None:
            stream.write(self.Unknown3)
        if self.Unknown4 is not None:
            stream.write(self.Unknown4)
        if self.Unknown5 is not None:
            stream.write(self.Unknown5)
        if self.Unknown6 is not None:
            stream.write(self.Unknown6)
    
    def clone(self) -> 'xxSubmesh':
        submesh = xxSubmesh()
        submesh.Unknown1 = bytes(self.Unknown1) if self.Unknown1 else None
        submesh.MaterialIndex = self.MaterialIndex
        submesh.FaceList = [face.clone() for face in self.FaceList]
        submesh.VertexList = [vertex.clone() for vertex in self.VertexList]
        submesh.Unknown2 = bytes(self.Unknown2) if self.Unknown2 else None
        if self.Vector2Lists:
            submesh.Vector2Lists = [[vec for vec in vl] for vl in self.Vector2Lists]
        submesh.Unknown3 = bytes(self.Unknown3) if self.Unknown3 else None
        submesh.Unknown4 = bytes(self.Unknown4) if self.Unknown4 else None
        submesh.Unknown5 = bytes(self.Unknown5) if self.Unknown5 else None
        submesh.Unknown6 = bytes(self.Unknown6) if self.Unknown6 else None
        return submesh


@dataclass
class xxMesh:
    """Represents a mesh."""
    NumVector2PerVertex: int = 0
    SubmeshList: List[xxSubmesh] = field(default_factory=list)
    VertexListDuplicateUnknown: bytes = b'\x00' * 8
    VertexListDuplicate: List[xxVertex] = field(default_factory=list)
    BoneList: List[xxBone] = field(default_factory=list)
    
    def write_to(self, stream: io.BytesIO):
        stream.write(struct.pack('<B', self.NumVector2PerVertex))
        
        for submesh in self.SubmeshList:
            submesh.write_to(stream)
        
        stream.write(struct.pack('<H', len(self.VertexListDuplicate)))
        stream.write(self.VertexListDuplicateUnknown)
        for vertex in self.VertexListDuplicate:
            vertex.write_to(stream)
        
        stream.write(struct.pack('<i', len(self.BoneList)))
        for bone in self.BoneList:
            bone.write_to(stream)
    
    def clone(self, submeshes: bool, vertex_list_dup: bool, bone_list: bool) -> 'xxMesh':
        mesh = xxMesh()
        mesh.NumVector2PerVertex = self.NumVector2PerVertex
        mesh.VertexListDuplicateUnknown = bytes(self.VertexListDuplicateUnknown)
        
        if submeshes:
            mesh.SubmeshList = [sm.clone() for sm in self.SubmeshList]
        if vertex_list_dup:
            mesh.VertexListDuplicate = [v.clone() for v in self.VertexListDuplicate]
        if bone_list:
            mesh.BoneList = [b.clone() for b in self.BoneList]
        
        return mesh


class xxFrame:
    """Represents a frame in the hierarchy."""
    
    def __init__(self):
        self.Name: str = ""
        self.Matrix: List[List[float]] = [[0.0]*4 for _ in range(4)]
        self.Unknown1: bytes = b''
        self.Bounds: Tuple[Tuple[float, float, float], Tuple[float, float, float]] = ((0, 0, 0), (0, 0, 0))
        self.Unknown2: bytes = b''
        self.Name2: Optional[str] = None
        self.Mesh: Optional[xxMesh] = None
        self.Parent: Optional['xxFrame'] = None
        self.children: List['xxFrame'] = []
    
    def init_children(self, count: int):
        self.children = []
    
    def add_child(self, child: 'xxFrame'):
        self.children.append(child)
        child.Parent = self
    
    def write_to(self, stream: io.BytesIO):
        writer = BinaryWriterExt(stream)
        writer.write_name(self.Name)
        stream.write(struct.pack('<i', len(self.children)))
        for row in self.Matrix:
            for val in row:
                stream.write(struct.pack('<f', val))
        stream.write(self.Unknown1)
        
        if self.Mesh is None:
            stream.write(struct.pack('<i', 0))
        else:
            stream.write(struct.pack('<i', len(self.Mesh.SubmeshList)))
        
        # Bounds
        for val in self.Bounds[0]:
            stream.write(struct.pack('<f', val))
        for val in self.Bounds[1]:
            stream.write(struct.pack('<f', val))
        
        stream.write(self.Unknown2)
        
        if self.Name2 is not None:
            writer.write_name(self.Name2)
        
        if self.Mesh is not None:
            self.Mesh.write_to(stream)
        
        for child in self.children:
            child.write_to(stream)
    
    def clone(self, mesh: bool, child_frames: bool, mat_translations: Optional[List[int]]) -> 'xxFrame':
        frame = xxFrame()
        frame.init_children(len(self.children))
        frame.Name = self.Name
        frame.Matrix = [row.copy() for row in self.Matrix]
        frame.Unknown1 = bytes(self.Unknown1)
        frame.Bounds = self.Bounds
        frame.Unknown2 = bytes(self.Unknown2)
        if self.Name2 is not None:
            frame.Name2 = self.Name2
        
        if mesh and self.Mesh is not None:
            frame.Mesh = self.Mesh.clone(True, True, True)
            if mat_translations is not None:
                for submesh in frame.Mesh.SubmeshList:
                    if submesh.MaterialIndex >= 0:
                        submesh.MaterialIndex = mat_translations[submesh.MaterialIndex]
        
        if child_frames:
            for child in self.children:
                frame.add_child(child.clone(mesh, True, mat_translations))
        
        return frame
    
    def __str__(self):
        return self.Name


class xxParser:
    """Parser for .xx files."""
    
    def __init__(self, stream: io.BytesIO, name: str = ""):
        self.Header: bytes = b''
        self.Frame: Optional[xxFrame] = None
        self.MaterialSectionUnknown: bytes = b''
        self.MaterialList: List[xxMaterial] = []
        self.TextureList: List[xxTexture] = []
        self.Footer: Optional[bytes] = None
        self.Name: str = name
        self.Format: int = 0
        
        self._parse(stream)
    
    def _parse(self, stream: io.BytesIO):
        reader = BinaryReaderExt(stream)
        
        # Determine format
        self.Format = 0
        format_buf = reader.read_bytes(5)
        
        if (format_buf[0] >= 0x01) and (struct.unpack('<i', format_buf[1:5])[0] == 0):
            self.Format = struct.unpack('<i', format_buf[0:4])[0]
        else:
            id_val = struct.unpack('<I', format_buf[0:4])[0]
            float_ids = [
                0x3F8F5C29, 0x3F90A3D7, 0x3F91EB85, 0x3F933333,
                0x3F947AE1, 0x3F95C28F, 0x3F970A3D, 0x3F99999A,
                0x3FA66666, 0x3FB33333
            ]
            if id_val in float_ids:
                self.Format = -1
        
        # Read header
        if self.Format >= 1:
            header_buf_len = 26
        else:
            header_buf_len = 21
        
        header_buf = bytearray(header_buf_len)
        header_buf[0:len(format_buf)] = format_buf
        remaining = reader.read_bytes(header_buf_len - len(format_buf))
        header_buf[len(format_buf):] = remaining
        self.Header = bytes(header_buf)
        
        # Parse frame
        self.Frame = self._parse_frame(reader)
        
        # Parse materials
        self.MaterialSectionUnknown = reader.read_bytes(4)
        num_materials = reader.read_int32()
        self.MaterialList = []
        for _ in range(num_materials):
            self.MaterialList.append(self._parse_material(reader))
        
        # Parse textures
        num_textures = reader.read_int32()
        self.TextureList = []
        for _ in range(num_textures):
            self.TextureList.append(self._parse_texture(reader))
        
        # Read footer if format >= 2
        if self.Format >= 2:
            self.Footer = reader.read_bytes(10)
        
        # Check if there's remaining data
        if len(reader.read_bytes(1)) > 0:
            print(f"Parsing {self.Name} finished before the end of the file - ignoring the rest")
    
    def _parse_frame(self, reader: BinaryReaderExt) -> xxFrame:
        frame = xxFrame()
        frame.Name = reader.read_name()
        
        num_child_frames = reader.read_int32()
        frame.init_children(num_child_frames)
        
        frame.Matrix = reader.read_matrix()
        
        if self.Format >= 7:
            frame.Unknown1 = reader.read_bytes(32)
        else:
            frame.Unknown1 = reader.read_bytes(16)
        
        num_submeshes = reader.read_int32()
        
        # Bounds
        min_bound = reader.read_vector3()
        max_bound = reader.read_vector3()
        frame.Bounds = (min_bound, max_bound)
        
        if self.Format >= 7:
            frame.Unknown2 = reader.read_bytes(64)
        else:
            frame.Unknown2 = reader.read_bytes(16)
        
        if self.Format >= 6:
            frame.Name2 = reader.read_name()
        
        if num_submeshes > 0:
            mesh = xxMesh()
            frame.Mesh = mesh
            mesh.NumVector2PerVertex = reader.read_byte()
            
            mesh.SubmeshList = []
            for _ in range(num_submeshes):
                submesh = xxSubmesh()
                
                if self.Format >= 7:
                    submesh.Unknown1 = reader.read_bytes(64)
                else:
                    submesh.Unknown1 = reader.read_bytes(16)
                
                submesh.MaterialIndex = reader.read_int32()
                submesh.FaceList = self._parse_face_list(reader)
                submesh.VertexList = self._parse_vertex_list(reader)
                
                if self.Format >= 7:
                    submesh.Unknown2 = reader.read_bytes(20)
                
                if mesh.NumVector2PerVertex > 0:
                    submesh.Vector2Lists = []
                    for _ in range(len(submesh.VertexList)):
                        vector_list = []
                        for _ in range(mesh.NumVector2PerVertex):
                            vector_list.append(reader.read_vector2())
                        submesh.Vector2Lists.append(vector_list)
                
                if self.Format >= 2:
                    submesh.Unknown3 = reader.read_bytes(100)
                
                if self.Format >= 7:
                    submesh.Unknown4 = reader.read_bytes(284)
                    
                    if self.Format >= 8:
                        format_byte = reader.read_byte()
                        null_frame = reader.read_name()
                        u5end = reader.read_bytes(12 + 4)
                        
                        encrypted_name = Utility.encrypt_name(null_frame)
                        u5_data = bytearray(1 + 4 + len(encrypted_name) + 12 + 4)
                        u5_data[0] = format_byte
                        struct.pack_into('<I', u5_data, 1, len(encrypted_name))
                        u5_data[1+4:1+4+len(encrypted_name)] = encrypted_name
                        u5_data[1+4+len(encrypted_name):] = u5end
                        submesh.Unknown5 = bytes(u5_data)
                else:
                    if self.Format >= 3:
                        submesh.Unknown4 = reader.read_bytes(64)
                    if self.Format >= 5:
                        submesh.Unknown5 = reader.read_bytes(20)
                    if self.Format >= 6:
                        submesh.Unknown6 = reader.read_bytes(28)
                
                mesh.SubmeshList.append(submesh)
            
            # Vertex list duplicate
            num_vertices_dup = reader.read_uint16()
            mesh.VertexListDuplicate = []
            mesh.VertexListDuplicateUnknown = reader.read_bytes(8)
            for _ in range(num_vertices_dup):
                mesh.VertexListDuplicate.append(self._parse_vertex(reader))
            
            # Bone list
            mesh.BoneList = self._parse_bone_list(reader)
        
        # Parse child frames
        for _ in range(num_child_frames):
            frame.add_child(self._parse_frame(reader))
        
        return frame
    
    def _parse_face_list(self, reader: BinaryReaderExt) -> List[xxFace]:
        num_faces_data = reader.read_int32()
        num_faces = num_faces_data // 3
        face_list = []
        for _ in range(num_faces):
            face = xxFace()
            face.VertexIndices = reader.read_uint16_array(3)
            face_list.append(face)
        return face_list
    
    def _parse_vertex_list(self, reader: BinaryReaderExt) -> List[xxVertex]:
        num_vertices = reader.read_int32()
        vertex_list = []
        for _ in range(num_vertices):
            vertex_list.append(self._parse_vertex(reader))
        return vertex_list
    
    def _parse_vertex(self, reader: BinaryReaderExt) -> xxVertex:
        if self.Format >= 4:
            vertex = xxVertexUShort()
            vertex.Index = reader.read_uint16()
        else:
            vertex = xxVertexInt()
            vertex.Index = reader.read_int32()
        
        vertex.Position = reader.read_vector3()
        vertex.Weights3 = reader.read_single_array(3)
        vertex.BoneIndices = reader.read_bytes(4)
        vertex.Normal = reader.read_vector3()
        vertex.UV = reader.read_single_array(2)
        
        if self.Format >= 4:
            vertex.Unknown1 = reader.read_bytes(20)
        
        return vertex
    
    def _parse_bone_list(self, reader: BinaryReaderExt) -> List[xxBone]:
        num_bones = reader.read_int32()
        bone_list = []
        for _ in range(num_bones):
            bone = xxBone()
            bone.Name = reader.read_name()
            bone.Index = reader.read_int32()
            bone.Matrix = reader.read_matrix()
            bone_list.append(bone)
        return bone_list
    
    def _parse_material(self, reader: BinaryReaderExt) -> xxMaterial:
        mat = xxMaterial()
        mat.Name = reader.read_name()
        mat.Diffuse = reader.read_color4()
        mat.Ambient = reader.read_color4()
        mat.Specular = reader.read_color4()
        mat.Emissive = reader.read_color4()
        mat.Power = reader.read_single()
        
        mat.Textures = []
        for _ in range(4):
            mat_tex = xxMaterialTexture()
            mat_tex.Name = reader.read_name()
            mat_tex.Unknown1 = reader.read_bytes(16)
            mat.Textures.append(mat_tex)
        
        if self.Format < 0:
            mat.Unknown1 = reader.read_bytes(4)
        else:
            mat.Unknown1 = reader.read_bytes(88)
        
        return mat
    
    def _parse_texture(self, reader: BinaryReaderExt) -> xxTexture:
        tex = xxTexture()
        tex.Name = reader.read_name()
        tex.Unknown1 = reader.read_bytes(4)
        tex.Width = reader.read_int32()
        tex.Height = reader.read_int32()
        tex.Depth = reader.read_int32()
        tex.MipLevels = reader.read_int32()
        tex.Format = reader.read_int32()
        tex.ResourceType = reader.read_int32()
        tex.ImageFileFormat = reader.read_int32()
        tex.Checksum = reader.read_byte()
        
        img_data_len = reader.read_int32()
        tex.ImageData = reader.read_bytes(img_data_len)
        
        return tex
    
    def write_to(self, stream: io.BytesIO):
        """Write the parsed data back to a stream."""
        stream.write(self.Header)
        
        if self.Frame is not None:
            self.Frame.write_to(stream)
        
        stream.write(self.MaterialSectionUnknown)
        stream.write(struct.pack('<i', len(self.MaterialList)))
        for mat in self.MaterialList:
            mat.write_to(stream)
        
        stream.write(struct.pack('<i', len(self.TextureList)))
        for tex in self.TextureList:
            tex.write_to(stream)
        
        if self.Footer is not None:
            stream.write(self.Footer)


# Example usage:
if __name__ == "__main__":
    import os
    from dotenv import load_dotenv
    load_dotenv()

    appdir = os.getenv('APPDIR')

    # Example of how to use the parser
    with open(os.path.join(appdir, 'out/sb03_00/mannequin_00.xx'), "rb") as f:
        parser = xxParser(f, "mannequin_00.xx")
        print(f"Format: {parser.Format}")
        print(f"Frame: {parser.Frame}")
        print(f"Materials: {len(parser.MaterialList)}")
        print(f"Textures: {len(parser.TextureList)}")
    pass
