#!/usr/bin/env python3
"""
Embeds a new TTF/OTF font file into NierikaDSPBinaryData.h / .cpp the same way
Projucer's BinaryData generator would (matching the existing Brandon_*_otf
entries), and (optionally) registers an accessor for it in
include/gui/EmbeddedFonts.h / .cpp.

Usage:
    python3 script/add_font.py path/to/Font.otf
    python3 script/add_font.py path/to/Font.otf --name JetBrainsMono
    python3 script/add_font.py path/to/Font.otf --no-getter
    python3 script/add_font.py path/to/Font.otf --dry-run
"""

import argparse
import sys
from pathlib import Path

from binary_data_common import (
    REPO_ROOT,
    HEADER_PATH,
    CPP_PATH,
    derive_identifier,
    compute_projucer_hash,
    next_binary_data_index,
    update_header,
    update_cpp,
)

EMBEDDED_FONTS_HEADER_PATH = REPO_ROOT / "include" / "gui" / "EmbeddedFonts.h"
EMBEDDED_FONTS_CPP_PATH = REPO_ROOT / "source" / "gui" / "EmbeddedFonts.cpp"

VALID_SUFFIXES = {".otf": "_otf", ".ttf": "_ttf"}
WRAP_WIDTH = 200


def format_byte_array(data: bytes, width: int = WRAP_WIDTH) -> str:
    parts = [str(b) for b in data]
    lines: list[str] = []
    current: list[str] = []
    current_len = 0
    for i, part in enumerate(parts):
        piece = part + ("," if i < len(parts) - 1 else "")
        if current_len + len(piece) > width and current:
            lines.append("".join(current))
            current = []
            current_len = 0
        current.append(piece)
        current_len += len(piece)
    if current:
        lines.append("".join(current))

    lines[0] = "{ " + lines[0]
    lines[-1] = lines[-1] + " };"
    return "\n".join(lines)


def build_font_binary_data_block(index: int, original_filename: str, identifier: str, byte_array_text: str) -> str:
    return (
        f"//================== {original_filename} ==================\n"
        f"static const unsigned char temp_binary_data_{index}[] =\n"
        f"{byte_array_text}\n\n"
        f"const char* {identifier} = (const char*) temp_binary_data_{index};\n"
    )


def update_embedded_fonts_header(text: str, getter_name: str) -> str:
    if f" {getter_name}(" in text:
        raise RuntimeError(f"'{getter_name}' is already declared in {EMBEDDED_FONTS_HEADER_PATH.name}")

    anchor = "\nprivate:"
    if anchor not in text:
        raise RuntimeError("Could not find the 'private:' anchor in EmbeddedFonts.h")
    new_line = f"\n    static juce::Font {getter_name}(float fontSize = 16.f);\nprivate:"
    return text.replace(anchor, new_line, 1)


def update_embedded_fonts_cpp(text: str, getter_name: str, identifier: str) -> str:
    anchor = "juce::Font EmbeddedFonts::buildFont("
    if anchor not in text:
        raise RuntimeError("Could not find EmbeddedFonts::buildFont in EmbeddedFonts.cpp")
    new_func = (
        f"juce::Font EmbeddedFonts::{getter_name}(float fontSize)\n"
        f"{{\n"
        f"    return buildFont(NierikaDSPBinaryData::{identifier}, NierikaDSPBinaryData::{identifier}Size, fontSize);\n"
        f"}}\n\n"
    )
    return text.replace(anchor, new_func + anchor, 1)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("font_path", type=Path, help="Path to the .otf/.ttf file to embed")
    parser.add_argument("--name", help="Override the generated identifier's base name (e.g. 'JetBrainsMono' -> JetBrainsMono_otf)")
    parser.add_argument("--no-getter", action="store_true", help="Don't add an accessor to include/gui/EmbeddedFonts.h/.cpp")
    parser.add_argument("--dry-run", action="store_true", help="Print what would change without writing any files")
    args = parser.parse_args()

    font_path: Path = args.font_path
    suffix = VALID_SUFFIXES.get(font_path.suffix.lower())
    if suffix is None:
        print(f"error: '{font_path}' is not a .otf or .ttf file", file=sys.stderr)
        return 1
    if not font_path.is_file():
        print(f"error: '{font_path}' does not exist", file=sys.stderr)
        return 1

    original_filename = font_path.name
    raw_bytes = font_path.read_bytes()

    identifier = derive_identifier(font_path, args.name, suffix)
    size_bytes = len(raw_bytes)
    byte_array_text = format_byte_array(raw_bytes)

    header_text = HEADER_PATH.read_text(encoding="utf-8")
    cpp_text = CPP_PATH.read_text(encoding="utf-8")

    new_header_text = update_header(header_text, identifier, size_bytes)

    index = next_binary_data_index(cpp_text)
    block = build_font_binary_data_block(index, original_filename, identifier, byte_array_text)
    new_cpp_text = update_cpp(cpp_text, identifier, original_filename, block, size_bytes)

    new_fonts_header_text = None
    new_fonts_cpp_text = None
    getter_name = None
    if not args.no_getter:
        base = identifier[:-len(suffix)] if identifier.endswith(suffix) else identifier
        getter_name = f"get{base[0].upper()}{base[1:]}"
        fonts_header_text = EMBEDDED_FONTS_HEADER_PATH.read_text(encoding="utf-8")
        fonts_cpp_text = EMBEDDED_FONTS_CPP_PATH.read_text(encoding="utf-8")
        new_fonts_header_text = update_embedded_fonts_header(fonts_header_text, getter_name)
        new_fonts_cpp_text = update_embedded_fonts_cpp(fonts_cpp_text, getter_name, identifier)

    print(f"identifier:   {identifier}")
    print(f"size (bytes): {size_bytes}")
    print(f"hash:         0x{compute_projucer_hash(identifier):08x}")
    if getter_name:
        print(f"font getter:  EmbeddedFonts::{getter_name}()")

    if args.dry_run:
        print("\n--dry-run: no files were modified")
        return 0

    HEADER_PATH.write_text(new_header_text, encoding="utf-8")
    CPP_PATH.write_text(new_cpp_text, encoding="utf-8")
    updated = [HEADER_PATH, CPP_PATH]
    if new_fonts_header_text is not None:
        EMBEDDED_FONTS_HEADER_PATH.write_text(new_fonts_header_text, encoding="utf-8")
        EMBEDDED_FONTS_CPP_PATH.write_text(new_fonts_cpp_text, encoding="utf-8")
        updated += [EMBEDDED_FONTS_HEADER_PATH, EMBEDDED_FONTS_CPP_PATH]

    print("\nUpdated " + ", ".join(str(p.relative_to(REPO_ROOT)) for p in updated))
    if getter_name:
        print(f"\nNext step: wire EmbeddedFonts::{getter_name}() into Theme::buildDefaultMonospaceFontSet() "
              f"(source/gui/Theme.cpp) for the weight(s) this font covers.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
