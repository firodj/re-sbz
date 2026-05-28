import struct
import os
from dotenv import load_dotenv
load_dotenv()

def read_anyl(fname):
    with open(fname, 'rb') as f:
        b = f.read(4)
        count = struct.unpack('<i', b)[0]
        for i in range(count):
            b = f.read(4)
            name_len = struct.unpack('<i', b)[0]
            b = f.read(name_len)
            name = b.decode('shift-jis').strip('\x00')
            print(name)
            b = f.read(4)
            value = struct.unpack('<i', b)[0]
            print(hex(value))

if __name__ == '__main__':
    appdir = os.getenv('APPDIR')
    read_anyl(os.path.join(appdir, "out/sb00_00/date_none_village_d.sl"))
    read_anyl(os.path.join(appdir, "out/sb00_00/date_none_village_d.vl"))
