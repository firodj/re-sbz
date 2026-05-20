def replace_tabs_with_spaces(input_file_path, output_file_path, tab_width=8):
    """
    Reads a file, converts all tabs to spaces based on the tab_width, 
    and writes the result to a new file.
    """
    try:
        with open(input_file_path, 'r', encoding='utf-8') as file:
            content = file.read()
        
        # Expand tabs to the designated space width dynamically
        converted_content = content.expandtabs(tab_width)
        
        with open(output_file_path, 'w', encoding='utf-8') as file:
            file.write(converted_content)
            
        print(f"Success! Converted tabs to {tab_width} spaces.")
        print(f"Output saved to: {output_file_path}")

    except FileNotFoundError:
        print(f"Error: The file '{input_file_path}' could not be found.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# --- Example Usage ---
# Replace 'code_with_tabs.txt' with your file name
if __name__ == '__main__':
    input_filename = 'tools/XXFormat1.txt'
    output_filename = 'tools/XXFormat1_t.txt'
    replace_tabs_with_spaces(input_filename, output_filename, tab_width=8)