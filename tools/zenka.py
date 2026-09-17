import sys


def to_shift_jis_fullwidth(text: str) -> str:
    result = []
    for char in text:
        code = ord(char)
        # Handle period and comma specially - convert to ideographic period/comma
        if code == 46:  # '.'
            result.append('。')  # ideographic period (U+3002)
        elif code == 44:  # ','
            result.append('、')  # ideographic comma (U+3001)
        # Check if the character is a standard ASCII alphanumeric/punctuation (33 to 126)
        elif 33 <= code <= 126:
            result.append(chr(code + 65248))
        # Handle standard space (32) separately, as its fullwidth equivalent is ideographic space (12288)
        elif code == 32:
            result.append(chr(12288))
        else:
            result.append(char)
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