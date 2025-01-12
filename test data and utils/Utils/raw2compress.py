import sys
from PIL import Image

def compress_rle(data):
    compressed_data = bytearray()
    i = 0

    while i < len(data):
        count = 1
        while i + 1 < len(data) and data[i] == data[i + 1] and count < 255:
            count += 1
            i += 1
        compressed_data.append(data[i])
        compressed_data.append(count)
        i += 1

    return compressed_data

def process_image(input_image_path, output_header_path):
    try:
        image = Image.open(input_image_path)
        image = image.resize((320, 240))
        image = image.convert('P', palette=Image.ADAPTIVE, colors=16)

        data = image.tobytes()

        # Pack two 4-bit pixels into one byte
        packed_data = bytearray()
        for i in range(0, len(data), 2):
            if i + 1 < len(data):
                byte = (data[i] & 0x0F) << 4 | (data[i + 1] & 0x0F)
            else:
                byte = (data[i] & 0x0F) << 4
            packed_data.append(byte)

        compressed_data = compress_rle(packed_data)
        
        # Create the palette array with format AAAARRRRGGGGBBBB
        palette = image.getpalette()[:48]  # Get first 16 colors (each color is represented by 3 bytes: RGB)
        palette_array = []
        
        for i in range(16):
            r = (palette[i * 3] >> 4) & 0xF
            g = (palette[i * 3 + 1] >> 4) & 0xF
            b = (palette[i * 3 + 2] >> 4) & 0xF
            a = 0xF  # Fixed alpha value
            color = (a << 12) | (r << 8) | (g << 4) | b
            palette_array.append(f"0x{color:04X}")

        header_content = f"const uint8_t compressed_data[] = {{\n"
        header_content += ", ".join(f"0x{byte:02X}" for byte in compressed_data)
        header_content += "\n};\n"
        header_content += f"const unsigned long compressed_size = {len(compressed_data)};\n\n"
        
        header_content += "const uint16_t palette[16] = {\n"
        header_content += ", ".join(palette_array)
        header_content += "\n};\n"

        with open(output_header_path, 'w') as f_out:
            f_out.write(header_content)

    except IOError as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_image> <output_header>")
        sys.exit(1)

    input_image_path = sys.argv[1]
    output_header_path = sys.argv[2]
    process_image(input_image_path, output_header_path)
