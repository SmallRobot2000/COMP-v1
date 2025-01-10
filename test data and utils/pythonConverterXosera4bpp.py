import sys
from PIL import Image

def convert_image_to_4bpp(image_path, output_base_name):
    img = Image.open(image_path).convert('P')
    img = img.quantize(colors=16, method=Image.FASTOCTREE)
    palette = img.getpalette()[:48]

    # Create C header file for the image
    with open(f'{output_base_name}.h', 'w') as header_file:
        header_file.write('#include <stdint.h>\n\n')
        
        # Write palette
        header_file.write(f'uint16_t {output_base_name}_palette[16] = {{\n')
        for i in range(16):
            r = palette[i * 3] >> 4
            g = palette[i * 3 + 1] >> 4
            b = palette[i * 3 + 2] >> 4
            header_file.write(f'    0xF{r:X}{g:X}{b:X},\n')
        header_file.write('};\n\n')
        
        # Write bitmap data
        header_file.write(f'uint8_t {output_base_name}_image[] = {{\n')
        pixel_data = img.load()
        for y in range(img.height):
            for x in range(0, img.width, 2):
                byte = (pixel_data[x, y] << 4) | (pixel_data[x + 1, y] if x + 1 < img.width else 0)
                header_file.write(f'    0x{byte:02X},\n')
        header_file.write('};\n')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_image> <output_base_name>")
        sys.exit(1)

    image_path = sys.argv[1]
    output_base_name = sys.argv[2]
    convert_image_to_4bpp(image_path, output_base_name)

