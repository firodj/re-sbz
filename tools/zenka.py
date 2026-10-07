import sys


def to_shift_jis_fullwidth(text: str) -> str:
    result = []
    skip_space = False
    for char in text:
        code = ord(char)
       
        # Handle period and comma specially - convert to ideographic period/comma
        if code == 46:  # '.'
            result.append('。')  # ideographic period (U+3002)
            if result[-3:len(result)] == ['。','。','。']:
                result = result[:-3] + ['…']
                skip_space = False
                continue
        elif code == 44:  # ','
            result.append('、')  # ideographic comma (U+3001)
        # Handle single quote - convert to fullwidth apostrophe
        elif code == 39:  # '''
            result.append('’')  # fullwidth apostrophe
        elif code == 33: # '!'
            result.append('！')
        elif code == 45: # '-'
            result.append('―')
        elif code == 63: # '?'
            result.append('？')
        else:
            # Handle standard space (32) separately, as its fullwidth equivalent is ideographic space (12288)
            
            if code == 32:
                if skip_space:
                    skip_space = False
                else:
                    result.append(chr(12288))
                continue

            skip_space = False
           
            # Check if the character is a standard ASCII alphanumeric/punctuation (33 to 126)
            if 33 <= code <= 126:
                result.append(chr(code + 65248))
            
            else:
                result.append(char)
            continue
                

        skip_space = True
    return "".join(result)


def main():
    if len(sys.argv) < 2:
        print("Usage: python zenka.py <input_file> [output_file]")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else None

    with open(input_file, 'r', encoding='utf-8') as f:
        text = f.read()

    fullwidth_str = to_shift_jis_fullwidth(text)

    if output_file:
        with open(output_file, 'wb') as f:
            f.write(fullwidth_str.encode('shift-jis'))
        print(f"Converted and saved to {output_file} (encoded as Shift_JIS)")
    else:
        print(fullwidth_str)


if __name__ == "__main__":
    main()