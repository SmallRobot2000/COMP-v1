from PIL import Image

def convert_image_to_8bpp(input_image_path, output_image_path, output_palette_path):
    # Open the image
    img = Image.open(input_image_path)
    img = img.resize((320, 240))  # Ensure the image is 320x240 pixels

    # Convert the image to 8-bit palette mode
    img = img.convert("P", palette=Image.ADAPTIVE, colors=256)

    # Save the 8bpp raw bitmap
    with open(output_image_path, "wb") as f:
        f.write(img.tobytes())

    # Generate the palette in AAAARRRRGGGGBBBB format
    palette = img.getpalette()
    palette_data = []
    for i in range(0, len(palette), 3):
        r = palette[i] >> 4
        g = palette[i+1] >> 4
        b = palette[i+2] >> 4
        a = 0xF
        color = (a << 12) | (r << 8) | (g << 4) | b
        palette_data.append(color)

    # Save the palette to a file
    with open(output_palette_path, "w") as f:
        for color in palette_data:
            f.write(f"{color:04X}\n")

# Usage
convert_image_to_8bpp("tataPravi.png", "tataOutput.raw", "tataPalette.pal")

