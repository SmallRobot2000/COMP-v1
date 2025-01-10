def read_raw_file(file_path):
    with open(file_path, 'rb') as file:
        data = file.read()
    return data

def write_assembly_like_file(file_path, data):
    with open(file_path, 'w') as file:
        for i in range(0, len(data), 2):
            # Read 2 bytes (16 bits) at a time
            word = data[i:i+2]
            if len(word) == 2:
                # Convert to hex and format as a word
                word_hex = f'{word[0]:02X}{word[1]:02X}'
                file.write(f'DW {word_hex}\n')
            elif len(word) == 1:
                # Handle case where there's an odd number of bytes
                word_hex = f'{word[0]:02X}00'
                file.write(f'DW {word_hex}\n')

def convert_raw_to_assembly(raw_file_path, assembly_file_path):
    data = read_raw_file(raw_file_path)
    write_assembly_like_file(assembly_file_path, data)

# Example usage
raw_file_path = 'tataOutput.raw'
assembly_file_path = 'tataOutput.asm'
convert_raw_to_assembly(raw_file_path, assembly_file_path)
