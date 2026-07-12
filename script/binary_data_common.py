"""
Shared helpers for scripts that add a new resource to NierikaDSPBinaryData.h / .cpp
the same way Projucer's BinaryData generator would (extern decl + size, a
temp_binary_data_N[] block, a case in the getNamedResource() hash switch, and
entries in namedResourceList[]/originalFilenames[]).

Used by add_svg_icon.py (text/SVG resources) and add_font.py (binary TTF/OTF
resources) — everything here is agnostic to how the resource content itself
is encoded into C++ source.
"""

import re
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
HEADER_PATH = REPO_ROOT / "include" / "NierikaDSPBinaryData.h"
CPP_PATH = REPO_ROOT / "source" / "NierikaDSPBinaryData.cpp"


def derive_identifier(path: Path, override: str | None, suffix: str) -> str:
    base = override if override else path.stem
    base = re.sub(r"[^0-9A-Za-z_]", "_", base)
    if not base:
        raise ValueError(f"Could not derive a valid identifier from '{path.name}'")
    if not override:
        # Match the repo's existing convention (ArrowDown_svg, Brandon_bld_otf, ...):
        # the identifier starts with an uppercase letter.
        base = base[0].upper() + base[1:]
    return f"{base}{suffix}"


def compute_projucer_hash(name: str) -> int:
    # Mirrors the hash Projucer bakes into BinaryData's getNamedResource() switch:
    # a Java String.hashCode()-style rolling hash over the resource variable name.
    h = 0
    for c in name:
        h = (31 * h + ord(c)) & 0xFFFFFFFF
    return h


def next_binary_data_index(cpp_text: str) -> int:
    indices = [int(m) for m in re.findall(r"temp_binary_data_(\d+)", cpp_text)]
    return (max(indices) + 1) if indices else 0


def insert_into_c_array(content: str, array_name: str, new_entry: str) -> str:
    pattern = re.compile(rf"(const char\* {array_name}\[\]\s*=\s*\{{\n)(.*?)(\n\}};)", re.DOTALL)
    m = pattern.search(content)
    if not m:
        raise RuntimeError(f"Could not find array '{array_name}' in binary data file")

    header, body, footer = m.group(1), m.group(2), m.group(3)
    lines = body.split("\n")
    lines[-1] = lines[-1].rstrip()
    if not lines[-1].endswith(","):
        lines[-1] += ","
    lines.append(f'    "{new_entry}"')
    new_body = "\n".join(lines)

    return content[:m.start()] + header + new_body + footer + content[m.end():]


def update_header(header_text: str, identifier: str, size_bytes: int) -> str:
    if f"extern const char*   {identifier};" in header_text:
        raise RuntimeError(f"'{identifier}' is already declared in {HEADER_PATH.name}")

    size_const_name = f"{identifier}Size"
    new_decl = f"    extern const char*   {identifier};\n    const int            {size_const_name} = {size_bytes};\n\n"

    anchor = "    // Number of elements in the namedResourceList and originalFileNames arrays."
    if anchor not in header_text:
        raise RuntimeError("Could not find the namedResourceList size anchor comment in the header")
    header_text = header_text.replace(anchor, new_decl + anchor, 1)

    def bump(m: re.Match) -> str:
        return f"const int namedResourceListSize = {int(m.group(1)) + 1};"

    header_text, count = re.subn(r"const int namedResourceListSize = (\d+);", bump, header_text)
    if count != 1:
        raise RuntimeError("Could not find/update namedResourceListSize in the header")

    return header_text


def update_cpp(cpp_text: str, identifier: str, original_filename: str, block: str, size_bytes: int) -> str:
    decl_anchor = "const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);"
    if cpp_text.count(decl_anchor) != 1:
        raise RuntimeError("Could not uniquely locate the getNamedResource forward declaration")
    cpp_text = cpp_text.replace(decl_anchor, block + "\n" + decl_anchor, 1)

    resource_hash = compute_projucer_hash(identifier)
    case_line = f"        case 0x{resource_hash:08x}:  numBytes = {size_bytes}; return {identifier};\n"
    default_anchor = "        default: break;"
    if cpp_text.count(default_anchor) != 1:
        raise RuntimeError("Could not uniquely locate the 'default: break;' switch anchor")
    cpp_text = cpp_text.replace(default_anchor, case_line + default_anchor, 1)

    cpp_text = insert_into_c_array(cpp_text, "namedResourceList", identifier)
    cpp_text = insert_into_c_array(cpp_text, "originalFilenames", original_filename)

    return cpp_text


def insert_before_class_close(text: str, class_name: str, new_line: str) -> str:
    pattern = re.compile(rf"(class {class_name}\s*\{{.*?)(\n\}};)", re.DOTALL)
    m = pattern.search(text)
    if not m:
        raise RuntimeError(f"Could not find the '{class_name}' class body")
    return text[:m.end(1)] + new_line + text[m.end(1):]
