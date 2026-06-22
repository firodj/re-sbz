import struct
from io import BytesIO
from dataclasses import dataclass, field
from typing import List, Optional
from binaryhelper import BinaryReader

try:
    import codecs
    _shift_jis = codecs.lookup('shift_jis')
except LookupError:
    _shift_jis = codecs.lookup('cp932')


class sdtParser:
    def __init__(self, stream):
        reader = BinaryReader(stream)

        format = reader.read_int32()
        print("format =", format)

        name = reader.read_name()
        print("name =", name)

        count = reader.read_int32()
        print("count =", count)

        for i in range(count):
            print("+ submember_18[%d]" % (i))
            self._parse_submember_18(reader)
    
    def _parse_submember_18(self, reader):
        field_0 = reader.read_int32()
        field_14 = reader.read_int32()
        count_C = reader.read_int32()
        print("  - field_0 =", field_0)
        print("  - field_14 =", field_14)
        print("  - count_C =", count_C)

        for i in range(count_C):
            print("  + submember_C[%d]" % (i))
            self._parse_submember_C(reader)

        count_4 = reader.read_int32()
        print("  - count_4 =", count_4)
        for i in range(count_4):
            print("  + submember_8[%d]" % (i))
            self._parse_submember_8(reader)

    def _parse_submember_C(self, reader):
        field_8 = reader.read_int32()
        name_4 = reader.read_name()

        print("    - field_8 =", field_8)
        print("    - name_4 =", name_4)

    def _parse_submember_8(self, reader):
        kind = reader.read_byte()
        print("    - kind = ", hex(kind))
        match kind:
            case 0:
                self._parse_case_0(reader)
            case 1:
                self._parse_case_1(reader)
            case 2:
                self._parse_case_2(reader)
            case 4:
                self._parse_case_4(reader)
            case 6:
                self._parse_case_6(reader)
            case 8:
                self._parse_case_8(reader)
            case 9:
                self._parse_case_9(reader)
            case 0xA:
                self._parse_case_A(reader)
            case 0x14:
                self._parse_case_14(reader)
            case 0x15:
                self._parse_case_15(reader)
            case 0x16:
                self._parse_case_16(reader)
            case 0x17:
                self._parse_case_17(reader)
            case 0x18:
                self._parse_case_18(reader)
            case _:
                raise Exception("unknown kind=%X" % (kind))
    
    # comment
    def _parse_case_0(self, reader):
        comment_0 = reader.read_name()
        print("    - comment_0 =", comment_0)

    def _parse_case_A(self, reader):
        name_0 = reader.read_name()
        print("    - name_0 =", name_0)
    
    def _parse_case_16(self, reader):
        name_0 = reader.read_name()
        print("    - name_0 =", name_0)

    def _parse_case_4(self, reader):
        field_0 = reader.read_int32()
        image_4 = reader.read_name()
        field_c = reader.read_int32()
        field_10 = reader.read_int32()
        right_14 = reader.read_int32()
        bottom_18 = reader.read_int32()

        print("    - field_0 =", field_0)
        print("    - image_4 =", image_4)
        print("    - field_c =", field_c)
        print("    - field_10 =", field_10)
        print("    - right_14 =", right_14)
        print("    - bottom_18 =", bottom_18)

    def _parse_case_8(self, reader):
        field_0 = reader.read_int32()
    
        print("    - play_sound_id_0 =", field_0)
    
    def _parse_case_17(self, reader):
        field_0 = reader.read_int32()
        byte_4 = reader.read_byte()

        print("    - field_0 =", field_0)
        print("    - byte_4 =", byte_4)

    def _parse_case_2(self, reader):
        blend_mode_0 = reader.read_byte()
        color_4 = reader.read_uint32()
        duration_8 = reader.read_int32()
    
        print("    - blend_mode_0 =", blend_mode_0)
        print("    - color_4 =", hex(color_4))
        print("    - duration_8 =", duration_8)

    def _parse_case_15(self, reader):
        field_0 = reader.read_uint32()
        field_4 = reader.read_uint32()
        field_8 = reader.read_int32()
    
        print("    - field_0 =", field_0)
        print("    - field_4 =", field_4)
        print("    - field_8 =", field_8)

    def _parse_case_6(self, reader):
        field_0 = reader.read_int32()
        name_4 = reader.read_name()
        byte_c = reader.read_byte()
        byte_d = reader.read_byte()

        print("    - sound_id_0 =", field_0)
        print("    - sound_4 =", name_4)
        print("    - byte_c =", byte_c)
        print("    - byte_d =", byte_d)

    def _parse_case_1(self, reader):
        dialog_id_0 = reader.read_int32()
        speaker_name_4 = reader.read_name_skip_1()
        sound_file_c = reader.read_name_skip_1()
        count_14 = reader.read_int32()

        print("    - dialog_id_0 =", dialog_id_0)
        print("    - speaker_name_4 =", speaker_name_4)
        print("    - sound_file_c =", sound_file_c)
        print("    - count_14 =", count_14)

        for i in range(count_14):
            print("    + struct_14[%d]" % (i))
            s14_byte_0 = reader.read_byte()
            print("      - byte_0 =", s14_byte_0)

            if s14_byte_0 != 0:
                s14_byte_1 = reader.read_byte()
                s14_byte_2 = reader.read_byte()
                s14_field_4 = reader.read_int32()
                s14_field_8 = reader.read_int32()

                print("      - byte_1 =", s14_byte_1)
                print("      - byte_2 =", s14_byte_2)
                print("      - field_4 =", s14_field_4)
                print("      - field_8 =", s14_field_8)
            
            s14_name_c = reader.read_name_skip_1()
            print("      - name_c =", s14_name_c)

        s8_count_0 = reader.read_int32()
        print("    + struct_8")
        print("      - count_0 =", s8_count_0)

        if s8_count_0 > 0:
            raise Exception("to be continue...")
    
    def _parse_case_9(self, reader):
        next_file_0 = reader.read_name()
        field_8 = reader.read_int32()

        print("    - next_file_0 =", next_file_0)
        print("    - field_8 =", field_8)

    def _parse_case_14(self, reader):
        print("    end.")

    def _parse_case_18(self, reader):
        field_0 = reader.read_int32()
        count_4 = reader.read_int32()

        print("    - field_0 =", field_0)
        print("    - count_4 =", count_4)

        for i in range(count_4):
            print("    + struct_10[%d]" % (i))
            s10_byte_5 = reader.read_byte()
            s10_byte_4 = reader.read_byte()
            s10_field_0 = reader.read_int32()
            s10_count_8 = reader.read_int32()

            print("      - field_0 =", s10_field_0)
            print("      - byte_4 =", s10_byte_4)
            print("      - byte_5 =", s10_byte_5)
            print("      - count_8 =", s10_count_8)

            print("      <<<<<<<<<<")
            for j in range(s10_count_8):            
                print("      + submember_8[%d]" % (j))
                self._parse_submember_8(reader)
            print("      >>>>>>>>>>")
    
# Example usage:
if __name__ == "__main__":
    import os
    from dotenv import load_dotenv
    load_dotenv()

    appdir = os.getenv('APPDIR')

    items = os.listdir(os.path.join(appdir, 'out/sb00_00/'))
    for item in items:
        if item.endswith('.sdt'):
            full_path = os.path.join(appdir, 'out/sb00_00/', item)
            with open(full_path, "rb") as f:
                print("========== ========= ==========")
                parser = sdtParser(f)
        
    # Example of how to use the parser
    #with open(os.path.join(appdir, 'out/sb00_00/talk_00_06_02.sdt'), "rb") as f:
    #    parser = sdtParser(f)
