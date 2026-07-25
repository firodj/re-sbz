# [ILLUSION] 
# GAME: RapeLay
# FileType: .pp
# unpack/repack illusion old games (.pp) format files
import os
from io import BytesIO

from dotenv import load_dotenv
load_dotenv()

def bin2ascii(b):
    res = []
    for x in b:
        if x < 0x20 or x > 0x7e:
            res.append('.')
        else:
            res.append(chr(x))
    return "".join(res)

def decode(s: bytes, encoding=None):
    if encoding is None:
        return bytes(map(lambda x: (-x) & 0xff, s))
    return bytes(map(lambda x: (-x) & 0xff, s)).decode(encoding)


def encode(s: bytes):
    return bytes(map(lambda x: (-x) & 0xff, s))


class FileItem:
    name: str
    size: int
    data: bytes | bytearray

    def __init__(self, name: bytes | str, size=0, data=None):
        self.name = decode(name, 'ascii') if isinstance(name, bytes) else name
        self.size = size
        self._data = data

    @property
    def data(self):
        return self._data

    @data.setter
    def data(self, d):
        self._data = b'' if d is None else decode(d)

    def save_as_file(self, dst_path):
        if not os.path.exists(dst_path):
            os.makedirs(dst_path, exist_ok=True)
        with open(os.path.join(dst_path, self.name), 'wb') as f:
            f.write(self.data)

    def __str__(self):
        return f'< {self.name} | {self.size} >'

    def __repr__(self):
        return self.__str__()


def unpack(path, s_dir):
    dir_, file = os.path.split(path)
    with open(path, 'rb') as f:
        pos = f.tell()
        count = int.from_bytes(f.read(4), 'little')
        print(f"{pos:#08x}: count = {count}")

        pos = f.tell()
        size = int.from_bytes(f.read(4), 'little')
        print(f"{pos:#08x}: size = {size}")
        out = []
        for i in range(count):  
            pos = f.tell()
            file_name = f.read(32).strip(b'\x00')
            item = FileItem(file_name)
            print(f"{pos:#08x}: file_name[{i}] = {item.name}")
            out.append(item)

        for i in range(count):
            pos = f.tell()
            file_size = int.from_bytes(f.read(4), 'little')
            print(f"{pos:#08x}: file_size[{i}] = {file_size}")
            out[i].size = file_size

        for i in range(count):
            pos = f.tell()
            file_data = f.read(out[i].size)
            print(f"{pos:#08x}: file_data[{i}] = {bin2ascii(file_data[:16])}")
            out[i].data = file_data

        s_path = os.path.join(s_dir, f'{os.path.splitext(file)[0]}')
        if not os.path.exists(s_path):
            os.makedirs(s_path, exist_ok=True)
        for i in out:
            print(i, s_path)
            i.save_as_file(s_path)


def unpack_pp_files(path: str, s_path: str):
    for file in filter(lambda x: x.endswith('.pp'), os.listdir(path)):
        print(file)
        unpack(os.path.join(path, file), s_path)


def pack(path, s_dir):
    pp_name = os.path.basename(path)
    count = 0
    size = 0
    writer = open(os.path.join(s_dir, f'{pp_name}.pp'), 'wb')

    file_names, file_sizes, data_writer = list(), list(), list()

    for file in os.listdir(path):
        with open(os.path.join(path, file), 'rb') as f:
            data = f.read()
            count += 1
            size += len(data)
            file_names.append(encode(file.encode('ascii')).ljust(32, b'\x00'))
            file_sizes.append(int.to_bytes(len(data), 4, 'little'))
            data_writer.append(encode(data))
    writer.write(int.to_bytes(count, 4, 'little'))
    writer.write(int.to_bytes(size, 4, 'little'))
    writer.write(b''.join(file_names))
    writer.write(b''.join(file_sizes))
    writer.write(b''.join(data_writer))

    writer.close()


def pack_pp_files(path: str, s_path: str):
    for pp_dir in os.listdir(path):
        if not os.path.isdir(os.path.join(path, pp_dir)):
            continue
        pack(os.path.join(path, pp_dir), s_path)

if __name__ == '__main__':
    appdir = os.getenv('SBZ_APPDIR')
    print(f"SBZ_APPDIR={appdir}")
    unpack(os.path.join(appdir, 'data', 'base.pp'), os.path.join(appdir, 'out'))