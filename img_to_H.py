from PIL import Image
import sys
import os


def rgb888_to_rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def convert_image(input_path, output_path, width, height, array_name="image_data"):

    img = Image.open(input_path).convert("RGB")

    # масштабирование с сохранением пропорций
    img_ratio = img.width / img.height
    target_ratio = width / height

    if img_ratio > target_ratio:
        new_height = height
        new_width = int(height * img_ratio)
    else:
        new_width = width
        new_height = int(width / img_ratio)

    img = img.resize((new_width, new_height), Image.LANCZOS)

    # центрированная обрезка
    left = (new_width - width) // 2
    top = (new_height - height) // 2
    right = left + width
    bottom = top + height

    img = img.crop((left, top, right, bottom))

    pixels = list(img.getdata())

    rgb565_pixels = [rgb888_to_rgb565(r, g, b) for r, g, b in pixels]

    with open(output_path, "w") as f:
        f.write("#pragma once\n")
        f.write("#include <stdint.h>\n\n")
        f.write(f"#define {array_name.upper()}_WIDTH {width}\n")
        f.write(f"#define {array_name.upper()}_HEIGHT {height}\n\n")

        f.write(f"const uint16_t {array_name}[{width*height}] = {{\n")

        for i, p in enumerate(rgb565_pixels):
            f.write(f"0x{p:04X}, ")
            if (i + 1) % width == 0:
                f.write("\n")

        f.write("};\n")


if __name__ == "__main__":

    if len(sys.argv) < 5:
        print("usage: python img2rgb565.py input.png width height output.h")
        sys.exit(1)

    input_img = sys.argv[1]
    width = int(sys.argv[2])
    height = int(sys.argv[3])
    output_file = sys.argv[4]

    name = os.path.splitext(os.path.basename(output_file))[0]

    convert_image(input_img, output_file, width, height, name)