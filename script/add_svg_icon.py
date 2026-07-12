#!/usr/bin/env python3
"""
Adds a new SVG icon to NierikaDSPBinaryData.h / .cpp the same way Projucer's
BinaryData generator would, and (optionally) registers an accessor for it in
include/gui/Icons.h.

Usage:
    python3 script/add_svg_icon.py path/to/icon.svg
    python3 script/add_svg_icon.py path/to/icon.svg --name Settings
    python3 script/add_svg_icon.py path/to/icon.svg --no-icon-getter
    python3 script/add_svg_icon.py path/to/icon.svg --dry-run
"""

import argparse
import re
import sys
import xml.etree.ElementTree as ET
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
    insert_before_class_close,
)

ICONS_HEADER_PATH = REPO_ROOT / "include" / "gui" / "Icons.h"

WRAP_WIDTH = 250


def normalize_default_colors(svg_text: str) -> tuple[str, int]:
    # SVGButton/SVGToggle/TextButton icons recolor at paint-time by string-replacing the
    # literal "#000000" inside each fill/stroke attribute (see Helpers.cpp::changeColor /
    # SVG_DEFAULT_COLOR) with the active theme colour. A literal "black" fill or stroke
    # wouldn't match that replacement, so icons must use "#000000" for their ink, whether
    # they're filled shapes (fill) or line-art icons (stroke).
    pattern = re.compile(r'(fill|stroke)=(["\'])black\2', re.IGNORECASE)
    normalized, count = pattern.subn(r'\1="#000000"', svg_text)
    return normalized, count


def escape_c_string(content: str) -> str:
    escaped = content.replace("\\", "\\\\").replace('"', '\\"')
    escaped = escaped.replace("\n", "\\n")
    return escaped


def wrap_escaped(escaped: str, width: int = WRAP_WIDTH) -> list[str]:
    chunks = []
    current = []
    current_len = 0
    i = 0
    n = len(escaped)
    while i < n:
        if escaped[i] == "\\" and i + 1 < n:
            piece = escaped[i:i + 2]
            i += 2
        else:
            piece = escaped[i]
            i += 1

        if current_len + len(piece) > width and current:
            chunks.append("".join(current))
            current = []
            current_len = 0

        current.append(piece)
        current_len += len(piece)

    if current:
        chunks.append("".join(current))

    return chunks or [""]


def build_binary_data_block(index: int, original_filename: str, identifier: str, escaped_lines: list[str]) -> str:
    quoted_lines = "\n".join(f'"{line}"' for line in escaped_lines)
    return (
        f"//================== {original_filename} ==================\n"
        f"static const unsigned char temp_binary_data_{index}[] =\n"
        f"{quoted_lines};\n\n"
        f"const char* {identifier} = (const char*) temp_binary_data_{index};\n"
    )


def update_icons_header(icons_text: str, identifier: str, getter_name: str) -> str:
    if f"::{identifier}" in icons_text:
        raise RuntimeError(f"'{identifier}' is already referenced in {ICONS_HEADER_PATH.name}")

    new_line = f'\n    static const char* {getter_name}() {{ return NierikaDSPBinaryData::{identifier}; }}'
    return insert_before_class_close(icons_text, "Icons", new_line)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("svg_path", type=Path, help="Path to the .svg file to add")
    parser.add_argument("--name", help="Override the generated identifier's base name (e.g. 'Settings' -> Settings_svg)")
    parser.add_argument("--no-icon-getter", action="store_true", help="Don't add an accessor to include/gui/Icons.h")
    parser.add_argument("--dry-run", action="store_true", help="Print what would change without writing any files")
    args = parser.parse_args()

    svg_path: Path = args.svg_path
    if svg_path.suffix.lower() != ".svg":
        print(f"error: '{svg_path}' is not a .svg file", file=sys.stderr)
        return 1
    if not svg_path.is_file():
        print(f"error: '{svg_path}' does not exist", file=sys.stderr)
        return 1

    original_filename = svg_path.name
    raw_content = svg_path.read_text(encoding="utf-8")

    normalized_content, color_fixes = normalize_default_colors(raw_content)
    if color_fixes:
        print(f"note: rewrote {color_fixes} literal fill/stroke=\"black\" occurrence(s) to \"#000000\" "
              f"(required for theme-driven recoloring)")

    try:
        ET.fromstring(normalized_content)
    except ET.ParseError as e:
        print(f"error: '{svg_path}' is not well-formed XML: {e}", file=sys.stderr)
        return 1

    identifier = derive_identifier(svg_path, args.name, "_svg")
    size_bytes = len(normalized_content.encode("utf-8"))
    escaped = escape_c_string(normalized_content)
    escaped_lines = wrap_escaped(escaped)

    header_text = HEADER_PATH.read_text(encoding="utf-8")
    cpp_text = CPP_PATH.read_text(encoding="utf-8")

    new_header_text = update_header(header_text, identifier, size_bytes)

    index = next_binary_data_index(cpp_text)
    block = build_binary_data_block(index, original_filename, identifier, escaped_lines)
    new_cpp_text = update_cpp(cpp_text, identifier, original_filename, block, size_bytes)

    new_icons_text = None
    getter_name = None
    if not args.no_icon_getter:
        icons_text = ICONS_HEADER_PATH.read_text(encoding="utf-8")
        base = identifier[:-len("_svg")] if identifier.endswith("_svg") else identifier
        getter_name = f"get{base[0].upper()}{base[1:]}"
        new_icons_text = update_icons_header(icons_text, identifier, getter_name)

    print(f"identifier:   {identifier}")
    print(f"size (bytes): {size_bytes}")
    print(f"hash:         0x{compute_projucer_hash(identifier):08x}")
    if getter_name:
        print(f"icon getter:  nierika::gui::Icons::{getter_name}()")

    if args.dry_run:
        print("\n--dry-run: no files were modified")
        return 0

    HEADER_PATH.write_text(new_header_text, encoding="utf-8")
    CPP_PATH.write_text(new_cpp_text, encoding="utf-8")
    if new_icons_text is not None:
        ICONS_HEADER_PATH.write_text(new_icons_text, encoding="utf-8")

    print(f"\nUpdated {HEADER_PATH.relative_to(REPO_ROOT)}, {CPP_PATH.relative_to(REPO_ROOT)}"
          + (f" and {ICONS_HEADER_PATH.relative_to(REPO_ROOT)}" if new_icons_text is not None else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
