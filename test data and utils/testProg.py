import os

# Input and output file names
input_file = "tataOutput.raw"  # Replace with your binary file name
output_file = "tataOutput.h"  # The generated C file

# Read the binary file
with open(input_file, "rb") as f:
    binary_data = f.read()

# Create the C array
with open(output_file, "w") as f:
    f.write("#include <stdint.h>\n\n")
    f.write(f"// Generated from {input_file}\n")
    f.write(f"const uint8_t test_sample[] = {{\n")

    # Write the binary data as a C array
    for i, byte in enumerate(binary_data):
        if i % 12 == 0:  # 12 bytes per line for better readability
            f.write("    ")
        f.write(f"0x{byte:02X}, ")
        if (i + 1) % 12 == 0:
            f.write("\n")
    
    # Close the array
    if len(binary_data) % 12 != 0:
        f.write("\n")
    f.write("};\n\n")

    # Write the size of the array
    f.write(f"const size_t test_sample_size = sizeof(test_sample);\n")

print(f"C array written to {output_file}")
