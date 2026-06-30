from binaryhelper import BinaryReader, BinaryWriter
import json
import io, sys
import argparse

try:
    import codecs
    _shift_jis = codecs.lookup('shift_jis')
except LookupError:
    _shift_jis = codecs.lookup('cp932')

def red_print(*args, **kwargs):
    print("\033[31m")
    print(*args)
    print("\033[0m")

def dbg_print(*args, **kwargs):
    if False:
        print(*args)

class sdtParser:
    def __init__(self, stream):
        self.ident = 0
        reader = BinaryReader(stream)

        format = reader.read_int32()
        name = reader.read_name()
        count = reader.read_int32()
        
        dbg_print("format =", format)
        dbg_print("name =", name)
        dbg_print("count =", count)

        sdt_script = dict(
            format=format,
            name=name,
            pages=[],
        )

        for i in range(count):
            dbg_print("+ submember_18[%d]" % (i))
            
            self.ident += 1
            
            sdt_page = self._parse_submember_18(reader)
            sdt_script['pages'].append(sdt_page)
            
            self.ident -= 1

        self.sdt_script = sdt_script
        self.consumed = stream.tell()
    
    def _parse_submember_18(self, reader):
        field_0 = reader.read_int32()
        field_14 = reader.read_int32()
        count_C = reader.read_int32()

        dbg_print(self.ident*"  " + "- field_0 =", field_0)
        dbg_print(self.ident*"  " + "- field_14 =", field_14)
        dbg_print(self.ident*"  " + "- count_C =", count_C)

        sdt_page = dict(
            field_0=field_0,
            field_14=field_14,
            labels=[],
            blocks=[],
        )

        for i in range(count_C):
            dbg_print(self.ident*"  " + "+ submember_C[%d]" % (i))
            self.ident += 1
            sdt_label = self._parse_submember_C(reader)
            sdt_page['labels'].append(sdt_label)
            self.ident -= 1

        count_4 = reader.read_int32()
        dbg_print(self.ident*"  " + "- count_4 =", count_4)
        for i in range(count_4):
            dbg_print(self.ident*"  " + "+ submember_8[%d]" % (i))
            self.ident += 1
            sdt_block = self._parse_submember_8(reader)
            sdt_page['blocks'].append(sdt_block)
            self.ident -= 1

        return sdt_page

    def _parse_submember_C(self, reader):
        field_8 = reader.read_int32()
        label = reader.read_name()

        dbg_print(self.ident*"  " + "- field_8 =", field_8)
        dbg_print(self.ident*"  " + "- label =", label)

        return dict(
            field_8=field_8,
            label=label
        )
       
    def _parse_submember_8(self, reader):
        kind = reader.read_byte()
        dbg_print(self.ident*"  " + "- kind = ", hex(kind))
        match kind:
            case 0:
                sdt_block = self._parse_case_0(reader)
            case 1:
                sdt_block = self._parse_case_1(reader)
            case 2:
                sdt_block = self._parse_case_2(reader)
            case 4:
                sdt_block = self._parse_case_4(reader)
            case 6:
                sdt_block = self._parse_case_6(reader)
            case 8:
                sdt_block = self._parse_case_8(reader)
            case 9:
                sdt_block = self._parse_case_9(reader)
            case 0xA:
                sdt_block = self._parse_case_A(reader)
            case 0x14:
                sdt_block = self._parse_case_14(reader)
            case 0x15:
                sdt_block = self._parse_case_15(reader)
            case 0x16:
                sdt_block = self._parse_case_16(reader)
            case 0x17:
                sdt_block = self._parse_case_17(reader)
            case 0x18:
                sdt_block = self._parse_case_18(reader)
            case _:
                raise NotImplementedError("unknown kind=%X" % (kind))
        
        return sdt_block
    
    # comment
    def _parse_case_0(self, reader):
        comment = reader.read_name()
        dbg_print(self.ident*"  " + "- comment =", comment)

        return dict(
            kind=0x0,
            comment=comment,
        )

    def _parse_case_A(self, reader):
        at_label = reader.read_name()
        dbg_print(self.ident*"  " + "- at_label =", at_label)

        return dict(
            kind=0xA,
            at_label=at_label,
        )
    
    def _parse_case_16(self, reader):
        goto_label = reader.read_name()
        dbg_print(self.ident*"  " + "- goto_label =", goto_label)

        return dict(
            kind=0x16,
            goto_label=goto_label,
        )

    def _parse_case_4(self, reader):
        field_0 = reader.read_int32()
        image = reader.read_name()
        field_C = reader.read_int32()
        field_10 = reader.read_int32()
        right = reader.read_int32()
        bottom = reader.read_int32()

        dbg_print(self.ident*"  " + "- field_0 =", field_0)
        dbg_print(self.ident*"  " + "- image =", image)
        dbg_print(self.ident*"  " + "- field_C =", field_C)
        dbg_print(self.ident*"  " + "- field_10 =", field_10)
        dbg_print(self.ident*"  " + "- right =", right)
        dbg_print(self.ident*"  " + "- bottom =", bottom)

        return dict(
            kind=0x4,
            field_0=field_0,
            image=image,
            field_C=field_C,
            field_10=field_10,
            right=right,
            bottom=bottom
        )

    def _parse_case_8(self, reader):
        play_sound_id = reader.read_int32()
    
        dbg_print(self.ident*"  " + "- play_sound_id =", play_sound_id)

        return dict(
            kind=0x8,
            play_sound_id=play_sound_id
        )
    
    def _parse_case_17(self, reader):
        field_0 = reader.read_int32()
        byte_4 = reader.read_byte()

        dbg_print(self.ident*"  " + "- field_0 =", field_0)
        dbg_print(self.ident*"  " + "- byte_4 =", byte_4)

        return dict(
            kind=0x17,
            field_0=field_0,
            byte_4=byte_4,
        )

    def _parse_case_2(self, reader):
        blend_mode = reader.read_byte()
        color = reader.read_uint32()
        duration = reader.read_int32()
    
        dbg_print(self.ident*"  " + "- blend_mode =", blend_mode)
        dbg_print(self.ident*"  " + "- color =", hex(color))
        dbg_print(self.ident*"  " + "- duration =", duration)

        return dict(
            kind=0x2,
            blend_mode=blend_mode,
            color=color,
            duration=duration,
        )

    def _parse_case_15(self, reader):
        field_0 = reader.read_uint32()
        field_4 = reader.read_uint32()
        field_8 = reader.read_int32()
    
        dbg_print(self.ident*"  " + "- field_0 =", field_0)
        dbg_print(self.ident*"  " + "- field_4 =", field_4)
        dbg_print(self.ident*"  " + "- field_8 =", field_8)

        return dict(
            kind=0x15,
            field_0=field_0,
            field_4=field_4,
            field_8=field_8,
        )

    def _parse_case_6(self, reader):
        sound_id = reader.read_int32()
        name = reader.read_name()
        byte_C = reader.read_byte()
        byte_D = reader.read_byte()

        dbg_print(self.ident*"  " + "- sound_id =", sound_id)
        dbg_print(self.ident*"  " + "- name =", name)
        dbg_print(self.ident*"  " + "- byte_C =", byte_C)
        dbg_print(self.ident*"  " + "- byte_D =", byte_D)

        return dict(
            kind=0x6,
            sound_id=sound_id,
            name=name,
            byte_C=byte_C,
            byte_D=byte_D,
        )

    def _parse_case_1(self, reader):
        dialog_id = reader.read_int32()
        speaker_name = reader.read_name_skip_1()
        sound_file = reader.read_name_skip_1()
        count_14 = reader.read_int32()

        dbg_print(self.ident*"  " + "- dialog_id =", dialog_id)
        dbg_print(self.ident*"  " + "- speaker_name =", speaker_name)
        dbg_print(self.ident*"  " + "- sound_file =", sound_file)
        dbg_print(self.ident*"  " + "- count_14 =", count_14)

        sdt_block_1=dict(
            kind=0x1,
            dialog_id=dialog_id,
            speaker_name=speaker_name,
            sound_file=sound_file,
            lines=[],
        )

        for i in range(count_14):
            dbg_print(self.ident*"  " + "+ struct_14[%d]" % (i))
            s14_byte_0 = reader.read_byte()

            self.ident += 1
            dbg_print(self.ident*"  " + "- byte_0 =", s14_byte_0)

            sdt_line=dict(byte_0=s14_byte_0)

            if s14_byte_0 != 0:
                s14_byte_1 = reader.read_byte()
                s14_byte_2 = reader.read_byte()
                s14_field_4 = reader.read_int32()
                s14_field_8 = reader.read_int32()

                dbg_print(self.ident*"  " + "- byte_1 =", s14_byte_1)
                dbg_print(self.ident*"  " + "- byte_2 =", s14_byte_2)
                dbg_print(self.ident*"  " + "- field_4 =", s14_field_4)
                dbg_print(self.ident*"  " + "- field_8 =", s14_field_8)

                sdt_line['byte_1'] = s14_byte_1
                sdt_line['byte_2'] = s14_byte_2
                sdt_line['field_4'] = s14_field_4
                sdt_line['field_8'] = s14_field_8
            
            s14_text = reader.read_name_skip_1()
            dbg_print(self.ident*"  " + "- text =", s14_text)
            sdt_line['text'] = s14_text

            sdt_block_1['lines'].append(sdt_line)

            self.ident -=1

        # Expect to always 0, otherwise not implemented error
        s8_count_0 = reader.read_int32()
        dbg_print(self.ident*"  " + "+ struct_8")

        self.ident += 1
        dbg_print(self.ident*"  " + "- count_0 =", s8_count_0)

        if s8_count_0 > 0:
            raise NotImplementedError("to be continue...")
        self.ident -= 1

        return sdt_block_1
    
    def _parse_case_9(self, reader):
        next_file = reader.read_name()
        field_8 = reader.read_int32()

        dbg_print(self.ident*"  " + "- next_file =", next_file)
        dbg_print(self.ident*"  " + "- field_8 =", field_8)

        return dict(
            kind=0x9,
            next_file=next_file,
            field_8=field_8,
        )

    def _parse_case_14(self, reader):
        dbg_print(self.ident*"  " + "- end.")

        return dict(
            kind=0x14
        )

    def _parse_case_18(self, reader):
        field_0 = reader.read_int32()
        count_4 = reader.read_int32()

        dbg_print(self.ident*"  " + "- field_0 =", field_0)
        dbg_print(self.ident*"  " + "- count_4 =", count_4)

        sdt_block_18 = dict(
            kind=0x18,
            field_0=field_0,
            pages=[],
        )

        for i in range(count_4):
            dbg_print(self.ident*"  " + "+ struct_10[%d]" % (i))
            self.ident += 1

            s10_byte_5 = reader.read_byte()
            s10_byte_4 = reader.read_byte()
            s10_field_0 = reader.read_int32()
            s10_count_8 = reader.read_int32()

            sdt_subpage=dict(
                field_0=s10_field_0,
                byte_4=s10_byte_4,
                byte_5=s10_byte_5,
                blocks=[]
            )

            dbg_print(self.ident*"  " + "- field_0 =", s10_field_0)
            dbg_print(self.ident*"  " + "- byte_4 =", s10_byte_4)
            dbg_print(self.ident*"  " + "- byte_5 =", s10_byte_5)
            dbg_print(self.ident*"  " + "- count_8 =", s10_count_8)

            for j in range(s10_count_8):            
                dbg_print(self.ident*"  " + "+ submember_8[%d]" % (j))
                self.ident += 1
                sdt_subblock = self._parse_submember_8(reader)
                sdt_subpage['blocks'].append(sdt_subblock)
                self.ident -= 1

            sdt_block_18['pages'].append(sdt_subpage)
            
            self.ident -=1
        
        return sdt_block_18
    
class SdtWriter:
    def __init__(self, jsondata, stream):
        sdt_script = json.loads(jsondata)
        writer = BinaryWriter(stream)

        format = sdt_script['format']
        name = sdt_script['name']
        count = len(sdt_script['pages'])

        writer.write_int32(format)
        writer.write_name(name)
        writer.write_int32(count)

        for i, sdt_page in enumerate(sdt_script['pages']):
            self._write_page(sdt_page, writer)

    def _write_page(self, sdt_page, writer):
        field_0 = sdt_page['field_0']
        field_14 = sdt_page['field_14']
        count_C = len(sdt_page['labels'])
        count_4 = len(sdt_page['blocks'])

        writer.write_int32(field_0)
        writer.write_int32(field_14)
        writer.write_int32(count_C)

        for i, sdt_label in enumerate(sdt_page['labels']):
            self._write_label(sdt_label, writer)

        writer.write_int32(count_4)

        for i, sdt_block in enumerate(sdt_page['blocks']):
            self._write_block(sdt_block, writer)

    def _write_label(self, sdt_label, writer):
        field_8 = sdt_label['field_8']
        label = sdt_label['label']

        writer.write_int32(field_8)
        writer.write_name(label)

    def _write_block(self, sdt_block, writer):
        kind = sdt_block['kind']

        writer.write_byte(kind)

        match kind:
            case 0:
                self._write_block_0(sdt_block, writer)
            case 1:
                self._write_block_1(sdt_block, writer)
            case 2:
                self._write_block_2(sdt_block, writer)
            case 4:
                self._write_block_4(sdt_block, writer)
            case 6:
                self._write_block_6(sdt_block, writer)
            case 8:
                self._write_block_8(sdt_block, writer)
            case 9:
                self._write_block_9(sdt_block, writer)
            case 0xA:
                self._write_block_A(sdt_block, writer)
            case 0x14:
                self._write_block_14(sdt_block, writer)
            case 0x15:
                self._write_block_15(sdt_block, writer)
            case 0x16:
                self._write_block_16(sdt_block, writer)
            case 0x17:
                self._write_block_17(sdt_block, writer)
            case 0x18:
                self._write_block_18(sdt_block, writer)

            case _:
                raise Exception("unknown kind=%X" % (kind))
            
    def _write_block_0(self, sdt_block, writer):
        comment = sdt_block['comment']

        writer.write_name(comment)

    def _write_block_A(self, sdt_block, writer):
        at_label = sdt_block['at_label']

        writer.write_name(at_label)

    def _write_block_16(self, sdt_block, writer):
        goto_label = sdt_block['goto_label']

        writer.write_name(goto_label)

    def _write_block_4(self, sdt_block, writer):
        field_0 = sdt_block['field_0']
        image = sdt_block['image']
        field_C = sdt_block['field_C']
        field_10 = sdt_block['field_10']
        right = sdt_block['right']
        bottom = sdt_block['bottom']

        writer.write_int32(field_0)
        writer.write_name(image)
        writer.write_int32(field_C)
        writer.write_int32(field_10)
        writer.write_int32(right)
        writer.write_int32(bottom)

    def _write_block_8(self, sdt_block, writer):
        play_sound_id = sdt_block['play_sound_id']

        writer.write_int32(play_sound_id)

    def _write_block_17(self, sdt_block, writer):
        field_0 = sdt_block['field_0']
        byte_4 = sdt_block['byte_4']

        writer.write_int32(field_0)
        writer.write_byte(byte_4)

    def _write_block_2(self, sdt_block, writer):
        blend_mode = sdt_block['blend_mode']
        color = sdt_block['color']
        duration = sdt_block['duration']

        writer.write_byte(blend_mode)
        writer.write_uint32(color)
        writer.write_int32(duration)

    def _write_block_15(self, sdt_block, writer):
        field_0 = sdt_block['field_0']
        field_4 = sdt_block['field_4']
        field_8 = sdt_block['field_8']

        writer.write_uint32(field_0)
        writer.write_uint32(field_4)
        writer.write_int32(field_8)

    def _write_block_6(self, sdt_block, writer):
        sound_id = sdt_block['sound_id']
        name = sdt_block['name']
        byte_C = sdt_block['byte_C']
        byte_D = sdt_block['byte_D']

        writer.write_int32(sound_id)
        writer.write_name(name)
        writer.write_byte(byte_C)
        writer.write_byte(byte_D)

    def _write_block_1(self, sdt_block, writer):
        dialog_id = sdt_block['dialog_id']
        speaker_name = sdt_block['speaker_name']
        sound_file = sdt_block['sound_file']

        count_14 = len(sdt_block['lines'])

        writer.write_int32(dialog_id)
        writer.write_name(speaker_name)
        writer.write_name(sound_file)

        writer.write_int32(count_14)

        for i, sdt_line in enumerate(sdt_block['lines']):
            lin_byte_0 = sdt_line['byte_0']
            writer.write_byte(lin_byte_0)

            if lin_byte_0 != 0:
                lin_byte_1 = sdt_line['byte_1']
                lin_byte_2 = sdt_line['byte_2']
                lin_field_4 = sdt_line['field_4']
                lin_field_8 = sdt_line['field_8']

                writer.write_byte(lin_byte_1)
                writer.write_byte(lin_byte_2)
                writer.write_int32(lin_field_4)
                writer.write_int32(lin_field_8)
            
            lin_text = sdt_line['text']
            writer.write_name(lin_text)

        s8_count_0 = 0
        writer.write_int32(s8_count_0)
                           
        if s8_count_0 > 0:
            raise NotImplementedError("to be continue...")
        
    def _write_block_9(self, sdt_block, writer):
        next_file = sdt_block['next_file']
        field_8 = sdt_block['field_8']

        writer.write_name(next_file)
        writer.write_int32(field_8)

    def _write_block_14(self, sdt_block, writer):
        return
    
    def _write_block_18(self, sdt_block, writer):
        field_0 = sdt_block['field_0']
        count_4 = len(sdt_block['pages'])

        writer.write_int32(field_0)
        writer.write_int32(count_4)

        for i, sdt_subpage in enumerate(sdt_block['pages']):
            s10_byte_5 = sdt_subpage['byte_5']
            s10_byte_4 = sdt_subpage['byte_4']
            s10_field_0 = sdt_subpage['field_0']
            s10_count_8 = len(sdt_subpage['blocks'])

            writer.write_byte(s10_byte_5)
            writer.write_byte(s10_byte_4)
            writer.write_int32(s10_field_0)
            writer.write_int32(s10_count_8)

            for j, sdt_subblock in enumerate(sdt_subpage['blocks']):
                self._write_block(sdt_subblock, writer)


def main():
    import os
    from dotenv import load_dotenv
    load_dotenv()

    islinux = sys.platform.startswith("linux")

    appdir = os.getenv('APPDIR_LINUX') if islinux else os.getenv('APPDIR')

    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--name", type=str, default='action_00_99_01.sdt', help="sdt file")
    parser.add_argument("-d", "--decode", action="store_true", help="decode sdt to json")
    parser.add_argument("-e", "--encode", action="store_true", help="encode json to sdt")
    parser.add_argument("-t", "--test", action="store_true", help="test decode encode sdt")
    parser.add_argument("-f", "--force", action="store_true", help="force")

    args = parser.parse_args()

    if False:
            
        items = os.listdir(os.path.join(appdir, 'data/sb00_00/'))
        for item in items:
            if item.endswith('.sdt'):
                full_path = os.path.join(appdir, 'data/sb00_00/', item)
                with open(full_path, "rb") as f:
                    print("========== ========= ==========")
                    parser = sdtParser(f)
    
    else:
        sdt_filename = args.name
        modfile_outpath = os.path.join(appdir, f'data/sb00_00/{sdt_filename}.mod')
        jsonfile_path = os.path.join(appdir, f'data/sb00_00/{sdt_filename}.json')

        if args.decode or args.test:

            with open(os.path.join(appdir, f'data/sb00_00/{sdt_filename}'), "rb") as f:
                parser = sdtParser(f)
                jsondata = json.dumps(parser.sdt_script, indent=4, ensure_ascii=False)

                # print(jsondata)
                print("consumed =", parser.consumed)

            if not args.test:
                if os.path.exists(jsonfile_path) and not args.force:
                    red_print(f"file exists, cannot overwrite: {jsonfile_path}")
                    return 1

                with open(jsonfile_path, "wb") as f:
                    f.write(jsondata.encode("utf-8"))
                    print("wrote to", jsonfile_path)
            
        elif args.encode:

            with open(jsonfile_path, "rb") as f:
                jsondata = f.read()

        if args.encode or args.test:
            outbuf = io.BytesIO()

            SdtWriter(jsondata, outbuf)

            size_wrote = outbuf.tell()
            print("size_wrote =", size_wrote)

            outbuf.seek(0)
            
            if args.encode:
                if os.path.exists(modfile_outpath) and not args.force:
                    red_print(f"file exists, cannot overwrite: {modfile_outpath}")
                    return 1

                with open(modfile_outpath, "wb") as f:
                    f.write(outbuf.getbuffer())
                    print("wrote to", modfile_outpath)

            elif args.test:
                print("=====================")
                outbuf.seek(0)
                new_parser = sdtParser(outbuf)
                print("consumed =", new_parser.consumed)
                #print(json.dumps(new_parser.sdt_script, indent=4, ensure_ascii=False))
    
    return 0

# Example usage:
if __name__ == "__main__":
    exitcode = main()
    sys.exit(exitcode)
    
