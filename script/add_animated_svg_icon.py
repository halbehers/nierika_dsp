#!/usr/bin/env python3
"""
Splits a single SVG containing either SMIL <animate>/<animateTransform> tags or a
CSS <style> block using @keyframes (targeting elements by #id, animating the CSS
`transform` property - the shape Figma exports when strokes have been outlined and
the animation is expressed as a scale/translate/rotate transform rather than a raw
geometry attribute) into N static SVG frame snapshots, one per shared keyframe
timestamp. Embeds all N frames into NierikaDSPBinaryData.h / .cpp the same way
add_svg_icon.py does for a single icon, and registers one accessor in
include/gui/AnimatedIcons.h returning a populated AnimatedIcon { frames,
durationInSeconds }. A file must use exactly one of the two mechanisms - mixing
SMIL and CSS animation in the same file is rejected.

v1 scope, both mechanisms: every animated element in the file must share the exact
same keyframe times and duration - a single Figma export is one timeline by
construction, so every animated element in it moves on the same schedule. Frames
are sampled at the exact declared keyframe instants only, never interpolated
between them, so easing (calcMode/keySplines/animation-timing-function) and
repeat/loop attributes never affect snapshot *geometry* and are dropped from the
frames themselves (looping is AnimatedSVG's job at playback time). They ARE,
however, inspected as an AnimatedIcon.easing detection signal (see below) - best
effort, soft-fail to None (not an error) on any mismatch or unrecognized curve,
since easing is a cosmetic nicety, not something that can make a frame set itself
incoherent the way a keyTimes/dur mismatch would.

SMIL rejected outright, with a specific error rather than a silent guess:
  - animated fill/stroke/stop-color (would freeze the icon's colour mid-animation,
    silently breaking this module's Theme-driven recolouring - see
    helpers::changeColor / SVG_DEFAULT_COLOR in Helpers.h/.cpp)
  - from/to/by instead of an explicit values="..." keyTimes="..." list
  - additive="sum", calcMode="paced", <animateMotion>, xlink:href-referenced targets

CSS @keyframes rejected outright:
  - any animated property other than `transform` (e.g. fill/opacity) - same
    colour-safety reasoning as the SMIL case, generalized: this script only ever
    freezes geometry/transform, never anything that would fight theme recolouring
  - any transform function other than translate/translateX/translateY/scale/
    scaleX/scaleY/rotate/skewX/skewY/matrix, or an angle/length with an
    unrecognized unit

Usage:
    python3 script/add_animated_svg_icon.py path/to/icon_anim.svg
    python3 script/add_animated_svg_icon.py path/to/icon_anim.svg --name Capture
    python3 script/add_animated_svg_icon.py path/to/icon_anim.svg --no-icon-getter
    python3 script/add_animated_svg_icon.py path/to/icon_anim.svg --dry-run
"""

import argparse
import copy
import math
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
from add_svg_icon import (
    normalize_default_colors,
    escape_c_string,
    wrap_escaped,
    build_binary_data_block,
)

ANIMATED_ICONS_HEADER_PATH = REPO_ROOT / "include" / "gui" / "AnimatedIcons.h"

SVG_NS = "http://www.w3.org/2000/svg"
XLINK_NS = "http://www.w3.org/1999/xlink"

ANIMATE_TAGS = ("animate", "animateTransform")

# fill/stroke/stop-color must stay static "#000000" so helpers::changeColor's literal
# string-swap keeps recolouring every frame - see the module-level docstring above.
DENIED_ATTRIBUTES = {"fill", "stroke", "stop-color"}


class SmilError(RuntimeError):
    pass


def local_name(tag: str) -> str:
    return tag.rsplit("}", 1)[-1]


def parse_semicolon_list(value: str) -> list[str]:
    return [item.strip() for item in value.split(";")]


def parse_duration(dur: str) -> float:
    dur = dur.strip()
    if dur.endswith("ms"):
        return float(dur[:-2]) / 1000.0
    if dur.endswith("s"):
        return float(dur[:-1])
    raise SmilError(f"unsupported dur=\"{dur}\" format (expected e.g. \"1.5s\" or \"500ms\")")


def describe(elem: ET.Element, position: int) -> str:
    tag = local_name(elem.tag)
    elem_id = elem.get("id")
    return f'<{tag} id="{elem_id}">' if elem_id else f"<{tag}> (document position {position})"


class AnimateBinding:
    def __init__(self, parent_path, attribute_name, values, key_times, dur, is_transform, transform_type,
                 target_description, calc_mode, key_splines_raw):
        self.parent_path = parent_path
        self.attribute_name = attribute_name
        self.values = values
        self.key_times = key_times
        self.dur = dur
        self.is_transform = is_transform
        self.transform_type = transform_type
        self.target_description = target_description
        self.calc_mode = calc_mode          # easing detection only - never affects snapshot geometry
        self.key_splines_raw = key_splines_raw  # easing detection only - raw string, parsed lazily/best-effort


def build_binding(parent: ET.Element, animate_elem: ET.Element, parent_path: tuple, position: int) -> AnimateBinding:
    tag = local_name(animate_elem.tag)
    is_transform = tag == "animateTransform"
    where = describe(animate_elem, position)

    if animate_elem.get(f"{{{XLINK_NS}}}href") is not None:
        raise SmilError(f"{where}: xlink:href-referenced targets are not supported; the script assumes each "
                         f"<animate>/<animateTransform> animates its own parent element")

    for forbidden in ("from", "to", "by"):
        if animate_elem.get(forbidden) is not None:
            raise SmilError(f"{where}: uses '{forbidden}=' instead of an explicit values=\"...\" keyTimes=\"...\" "
                             f"list; re-export with explicit keyframe values")

    if animate_elem.get("additive") == "sum":
        raise SmilError(f"{where}: additive=\"sum\" is not supported; the frozen value would need composing with "
                         f"the parent's existing transform")

    # SMIL spec default calcMode is "linear" when the attribute is absent.
    calc_mode = animate_elem.get("calcMode", "linear")
    if calc_mode == "paced":
        raise SmilError(f"{where}: calcMode=\"paced\" is not supported")

    if is_transform:
        transform_type = animate_elem.get("type")
        if not transform_type:
            raise SmilError(f"{where}: <animateTransform> is missing a 'type' attribute")
        attribute_name = "transform"
    else:
        transform_type = None
        attribute_name = animate_elem.get("attributeName")
        if not attribute_name:
            raise SmilError(f"{where}: missing attributeName")
        if attribute_name in DENIED_ATTRIBUTES:
            raise SmilError(f"{where}: animated '{attribute_name}' is not supported (colour attributes must stay "
                             f"static as \"#000000\" so theme-driven recolouring keeps working every frame); keep "
                             f"colour static and only animate geometry/transform attributes")

    values_attr = animate_elem.get("values")
    if not values_attr:
        raise SmilError(f"{where}: missing a values=\"...\" list")
    values = parse_semicolon_list(values_attr)

    key_times_attr = animate_elem.get("keyTimes")
    if key_times_attr:
        key_times = [float(t) for t in parse_semicolon_list(key_times_attr)]
    else:
        # SMIL spec default: evenly-spaced fractions over [0,1] when keyTimes is omitted.
        n = len(values)
        key_times = [i / (n - 1) for i in range(n)] if n > 1 else [0.0]

    if len(key_times) != len(values):
        raise SmilError(f"{where}: keyTimes has {len(key_times)} entries but values has {len(values)}")

    dur_attr = animate_elem.get("dur")
    if not dur_attr:
        raise SmilError(f"{where}: missing a dur=\"...\" attribute")

    return AnimateBinding(parent_path, attribute_name, values, key_times, dur_attr, is_transform, transform_type,
                           describe(parent, position), calc_mode, animate_elem.get("keySplines"))


def collect_bindings(root: ET.Element) -> list[AnimateBinding]:
    bindings = []
    position = [0]

    def walk(elem: ET.Element, path: tuple):
        for i, child in enumerate(list(elem)):
            position[0] += 1
            if local_name(child.tag) in ANIMATE_TAGS:
                bindings.append(build_binding(elem, child, path, position[0]))
            walk(child, path + (i,))

    walk(root, ())
    return bindings


def validate_shared_schedule(bindings: list[AnimateBinding]) -> tuple[list[float], float]:
    if not bindings:
        raise SmilError("no <animate>/<animateTransform> elements found in this SVG")

    first = bindings[0]
    for other in bindings[1:]:
        # Tolerance wide enough to absorb sub-0.1% export rounding noise (e.g. Figma
        # emitting 24.99% on one element's keyframe and 25% on another's for what is
        # conceptually the same instant) without masking a genuinely different schedule.
        same_key_times = len(other.key_times) == len(first.key_times) and all(
            abs(a - b) <= 1e-3 for a, b in zip(first.key_times, other.key_times)
        )
        if not same_key_times:
            raise SmilError(
                f"keyframe schedule mismatch: {first.target_description} (attributeName={first.attribute_name}, "
                f"keyTimes={first.key_times}) does not share a keyframe schedule with {other.target_description} "
                f"(attributeName={other.attribute_name}, keyTimes={other.key_times}); this script requires every "
                f"animated element in the file to use one common keyTimes/dur schedule - re-export with "
                f"synchronized keyframes"
            )
        if other.dur.strip() != first.dur.strip():
            raise SmilError(
                f"keyframe schedule mismatch: {first.target_description} has dur=\"{first.dur}\" but "
                f"{other.target_description} has dur=\"{other.dur}\"; this script requires every animated element "
                f"in the file to share the same dur"
            )

    frame_count = len(first.key_times)
    if frame_count < 2:
        raise SmilError("the shared keyframe schedule has fewer than 2 keyframes")

    return first.key_times, parse_duration(first.dur)


def resolve(root: ET.Element, path: tuple) -> ET.Element:
    elem = root
    for i in path:
        elem = list(elem)[i]
    return elem


def find_matching_animate_child(parent: ET.Element, binding: AnimateBinding):
    # Matched by what it animates, not by position - a frame's earlier removals must
    # never shift the index of a not-yet-processed sibling binding under the same parent.
    for child in list(parent):
        if local_name(child.tag) not in ANIMATE_TAGS:
            continue
        if binding.is_transform:
            if local_name(child.tag) == "animateTransform" and child.get("type") == binding.transform_type:
                return child
        elif local_name(child.tag) == "animate" and child.get("attributeName") == binding.attribute_name:
            return child
    return None


def snapshot_frames(root: ET.Element, bindings: list[AnimateBinding], frame_count: int) -> list[str]:
    frames = []
    for i in range(frame_count):
        frame_root = copy.deepcopy(root)

        for binding in bindings:
            parent = resolve(frame_root, binding.parent_path)
            animate_elem = find_matching_animate_child(parent, binding)
            if animate_elem is None:
                raise SmilError(f"internal error: could not re-locate {binding.target_description}'s "
                                 f"{binding.attribute_name} animation while snapshotting frame {i}")

            value = binding.values[i]
            if binding.is_transform:
                parent.set("transform", f"{binding.transform_type}({value})")
            else:
                parent.set(binding.attribute_name, value)

            parent.remove(animate_elem)

        frame_text = ET.tostring(frame_root, encoding="unicode")
        ET.fromstring(frame_text)  # sanity check: must re-parse cleanly before it's embedded
        frames.append(frame_text)

    return frames


########################################################################################
# CSS @keyframes parsing (alternative to the SMIL <animate> path above)
########################################################################################

# (a, b, c, d, e, f) representing the SVG/CSS 2D affine matrix
#   | a c e |
#   | b d f |
#   | 0 0 1 |
Matrix = tuple
IDENTITY_MATRIX: Matrix = (1.0, 0.0, 0.0, 1.0, 0.0, 0.0)

CSS_LENGTH_RE = re.compile(r'^(-?[\d.]+)(px)?$')
CSS_ANGLE_RE = re.compile(r'^(-?[\d.]+)(deg|rad|grad|turn)$')
CSS_TRANSFORM_FUNC_RE = re.compile(r'([a-zA-Z]+)\(([^)]*)\)')


def parse_css_length(value: str) -> float:
    value = value.strip()
    m = CSS_LENGTH_RE.match(value)
    if not m:
        raise SmilError(f"unsupported CSS length '{value}' (expected a plain number, optionally suffixed 'px')")
    return float(m.group(1))


def parse_css_angle(value: str) -> float:
    """Returns radians."""
    value = value.strip()
    if value == "0":
        return 0.0
    m = CSS_ANGLE_RE.match(value)
    if not m:
        raise SmilError(f"unsupported CSS angle '{value}' (expected a number suffixed 'deg'/'rad'/'grad'/'turn')")
    number, unit = float(m.group(1)), m.group(2)
    if unit == "rad":
        return number
    if unit == "deg":
        return math.radians(number)
    if unit == "grad":
        return number * math.pi / 200.0
    return number * 2.0 * math.pi  # turn


def matrix_multiply(m1: Matrix, m2: Matrix) -> Matrix:
    a1, b1, c1, d1, e1, f1 = m1
    a2, b2, c2, d2, e2, f2 = m2
    return (
        a1 * a2 + c1 * b2,
        b1 * a2 + d1 * b2,
        a1 * c2 + c1 * d2,
        b1 * c2 + d1 * d2,
        a1 * e2 + c1 * f2 + e1,
        b1 * e2 + d1 * f2 + f1,
    )


def transform_function_matrix(name: str, args: list[str]) -> Matrix:
    if name == "translate":
        tx = parse_css_length(args[0])
        ty = parse_css_length(args[1]) if len(args) > 1 else 0.0
        return (1.0, 0.0, 0.0, 1.0, tx, ty)
    if name == "translatex":
        return (1.0, 0.0, 0.0, 1.0, parse_css_length(args[0]), 0.0)
    if name == "translatey":
        return (1.0, 0.0, 0.0, 1.0, 0.0, parse_css_length(args[0]))
    if name == "scale":
        sx = float(args[0])
        sy = float(args[1]) if len(args) > 1 else sx
        return (sx, 0.0, 0.0, sy, 0.0, 0.0)
    if name == "scalex":
        return (float(args[0]), 0.0, 0.0, 1.0, 0.0, 0.0)
    if name == "scaley":
        return (1.0, 0.0, 0.0, float(args[0]), 0.0, 0.0)
    if name == "rotate":
        theta = parse_css_angle(args[0])
        c, s = math.cos(theta), math.sin(theta)
        return (c, s, -s, c, 0.0, 0.0)
    if name == "skewx":
        return (1.0, 0.0, math.tan(parse_css_angle(args[0])), 1.0, 0.0, 0.0)
    if name == "skewy":
        return (1.0, math.tan(parse_css_angle(args[0])), 0.0, 1.0, 0.0, 0.0)
    if name == "matrix":
        a, b, c, d, e, f = (float(v) for v in args)
        return (a, b, c, d, e, f)
    raise SmilError(f"unsupported CSS transform function '{name}(...)'")


def parse_css_transform(value: str) -> Matrix:
    matches = list(CSS_TRANSFORM_FUNC_RE.finditer(value))
    if not matches:
        raise SmilError(f"could not parse CSS transform value '{value}'")
    matrix = IDENTITY_MATRIX
    for m in matches:
        name = m.group(1).lower()
        args = [a.strip() for a in m.group(2).split(",")] if m.group(2).strip() else []
        matrix = matrix_multiply(matrix, transform_function_matrix(name, args))
    return matrix


def format_number(v: float) -> str:
    rounded = round(v, 6)
    if rounded == int(rounded):
        return str(int(rounded))
    return f"{rounded:.6f}".rstrip('0').rstrip('.')


def format_matrix(m: Matrix) -> str:
    return f"matrix({' '.join(format_number(v) for v in m)})"


def split_top_level_blocks(css_text: str) -> list[tuple[str, str]]:
    """Splits `header { body } header { body } ...` into (header, body) pairs, tracking
    brace depth so a body that itself contains nested `{...}` blocks (e.g. a @keyframes
    rule's percentage steps) is captured whole rather than truncated at the first `}`."""
    blocks = []
    i = 0
    n = len(css_text)
    while i < n:
        while i < n and css_text[i].isspace():
            i += 1
        if i >= n:
            break
        start = i
        depth = 0
        header_end = None
        while i < n:
            if css_text[i] == '{':
                if depth == 0:
                    header_end = i
                depth += 1
            elif css_text[i] == '}':
                depth -= 1
                if depth == 0:
                    i += 1
                    break
            i += 1
        if header_end is None:
            break
        header = css_text[start:header_end].strip()
        body = css_text[header_end + 1:i - 1].strip()
        blocks.append((header, body))
    return blocks


def parse_declarations(body: str) -> dict[str, str]:
    props = {}
    for decl in body.split(";"):
        decl = decl.strip()
        if not decl:
            continue
        if ":" not in decl:
            raise SmilError(f"malformed CSS declaration '{decl}'")
        prop, _, value = decl.partition(":")
        props[prop.strip().lower()] = value.strip()
    return props


def parse_percentage(header: str) -> float:
    header = header.strip().lower()
    if header == "from":
        return 0.0
    if header == "to":
        return 1.0
    if not header.endswith("%"):
        raise SmilError(f"unsupported keyframe selector '{header}' (expected a percentage, 'from', or 'to')")
    return float(header[:-1]) / 100.0


def find_style_text(root: ET.Element) -> str:
    for elem in root.iter():
        if local_name(elem.tag) == "style" and elem.text:
            return elem.text
    return ""


def index_elements_by_id(root: ET.Element) -> dict[str, tuple[tuple, ET.Element]]:
    result = {}

    def walk(elem: ET.Element, path: tuple):
        for i, child in enumerate(list(elem)):
            if "id" in child.attrib:
                result[child.attrib["id"]] = (path + (i,), child)
            walk(child, path + (i,))

    walk(root, ())
    return result


class CssBinding:
    def __init__(self, path, key_times, dur, matrices, target_description, step_timing_functions, shorthand_timing):
        self.path = path
        self.key_times = key_times
        self.dur = dur
        self.attribute_name = "transform"  # for validate_shared_schedule's error text only
        self.matrices = matrices
        self.target_description = target_description
        self.step_timing_functions = step_timing_functions  # {pct: str | None} - easing detection only
        self.shorthand_timing = shorthand_timing             # str | None - easing detection only


def collect_css_bindings(root: ET.Element, style_text: str) -> list[CssBinding]:
    keyframes: dict[str, dict[float, Matrix]] = {}
    keyframe_timing_functions: dict[str, dict[float, str | None]] = {}
    animations: dict[str, tuple[str, str, str | None]] = {}  # elem_id -> (keyframes_name, dur, shorthand_timing)

    for header, body in split_top_level_blocks(style_text):
        if header.startswith("@keyframes"):
            name = header[len("@keyframes"):].strip()
            steps: dict[float, Matrix] = {}
            timing_functions: dict[float, str | None] = {}
            for step_header, step_body in split_top_level_blocks(body):
                pct = parse_percentage(step_header)
                props = parse_declarations(step_body)
                for prop in props:
                    if prop not in ("transform", "animation-timing-function"):
                        raise SmilError(f"@keyframes {name} {step_header}: animated '{prop}' is not supported "
                                         f"(only 'transform' keyframe animation is supported - colour/opacity/etc. "
                                         f"must stay static so theme-driven recolouring keeps working every frame)")
                if "transform" not in props:
                    raise SmilError(f"@keyframes {name} {step_header}: missing a transform declaration")
                steps[pct] = parse_css_transform(props["transform"])
                timing_functions[pct] = props.get("animation-timing-function")
            keyframes[name] = steps
            keyframe_timing_functions[name] = timing_functions
        elif header.startswith("#"):
            elem_id = header[1:].strip()
            props = parse_declarations(body)
            animation = props.get("animation")
            if not animation:
                continue
            parts = animation.split()
            if len(parts) < 2:
                raise SmilError(f"#{elem_id}: malformed 'animation' shorthand \"{animation}\"")
            shorthand_timing = parts[2] if len(parts) > 2 else None
            animations[elem_id] = (parts[0], parts[1], shorthand_timing)
        # Any other top-level rule (plain non-animation CSS) is left alone - not our concern.

    id_index = index_elements_by_id(root)

    bindings = []
    for elem_id, (kf_name, dur, shorthand_timing) in animations.items():
        if kf_name not in keyframes:
            raise SmilError(f"#{elem_id}: references @keyframes '{kf_name}' which was not found")
        if elem_id not in id_index:
            raise SmilError(f"#{elem_id}: no element with this id was found in the SVG")

        path, elem = id_index[elem_id]
        steps = keyframes[kf_name]
        key_times = sorted(steps.keys())
        matrices = [steps[t] for t in key_times]

        bindings.append(CssBinding(path, key_times, dur, matrices, f'<{local_name(elem.tag)} id="{elem_id}">',
                                    keyframe_timing_functions[kf_name], shorthand_timing))

    return bindings


def remove_style_elements(root: ET.Element) -> None:
    for parent in root.iter():
        for child in list(parent):
            if local_name(child.tag) == "style":
                parent.remove(child)


def snapshot_frames_css(root: ET.Element, bindings: list[CssBinding], frame_count: int) -> list[str]:
    frames = []
    for i in range(frame_count):
        frame_root = copy.deepcopy(root)

        for binding in bindings:
            elem = resolve(frame_root, binding.path)
            elem.set("transform", format_matrix(binding.matrices[i]))

        remove_style_elements(frame_root)

        frame_text = ET.tostring(frame_root, encoding="unicode")
        ET.fromstring(frame_text)  # sanity check: must re-parse cleanly before it's embedded
        frames.append(frame_text)

    return frames


########################################################################################
# Easing detection - best-effort, soft-fail to None. Populates AnimatedIcon.easing but
# never blocks an otherwise-valid import (unlike keyTimes/dur mismatches, which are
# hard errors because they'd make the frame *set* itself incoherent).
########################################################################################

# Standard CSS Easing Functions Level 1 control points.
EASING_PRESETS = {
    "EASE_IN": (0.42, 0.0, 1.0, 1.0),
    "EASE_OUT": (0.0, 0.0, 0.58, 1.0),
    "EASE_IN_OUT": (0.42, 0.0, 0.58, 1.0),
}
# Reuses the script's existing keyTimes-comparison tolerance (validate_shared_schedule)
# for consistency, rather than inventing a separate epsilon.
BEZIER_EPSILON = 1e-3


def match_bezier_preset(bezier: tuple[float, float, float, float]) -> str | None:
    for name, preset in EASING_PRESETS.items():
        if all(abs(a - b) <= BEZIER_EPSILON for a, b in zip(bezier, preset)):
            return name
    return None


def parse_key_splines(value: str) -> list[tuple[float, float, float, float]]:
    segments = []
    for seg in parse_semicolon_list(value):
        parts = [p.strip() for p in seg.split(",")]
        if len(parts) != 4:
            raise ValueError(f"malformed keySplines segment '{seg}'")
        segments.append(tuple(float(p) for p in parts))
    return segments


def detect_smil_easing(bindings: list[AnimateBinding]) -> str | None:
    detected_names = set()
    for binding in bindings:
        if binding.calc_mode == "discrete":
            detected_names.add("FLAT")
        elif binding.calc_mode == "linear":
            detected_names.add("LINEAR")
        elif binding.calc_mode == "spline":
            if not binding.key_splines_raw:
                return None
            try:
                segments = parse_key_splines(binding.key_splines_raw)
            except ValueError:
                return None
            if not segments:
                return None
            first_segment = segments[0]
            if not all(all(abs(a - b) <= BEZIER_EPSILON for a, b in zip(first_segment, seg)) for seg in segments[1:]):
                return None  # this element's own segments don't even agree with each other
            match = match_bezier_preset(first_segment)
            if match is None:
                return None
            detected_names.add(match)
        else:
            return None  # unrecognized calcMode

    if len(detected_names) != 1:
        return None
    return next(iter(detected_names))


CSS_KEYWORD_BEZIERS = {
    "ease-in": (0.42, 0.0, 1.0, 1.0),
    "ease-out": (0.0, 0.0, 0.58, 1.0),
    "ease-in-out": (0.42, 0.0, 0.58, 1.0),
    "ease": (0.25, 0.1, 0.25, 1.0),  # the CSS default - matches none of our 3 presets, resolves to None
}

CUBIC_BEZIER_RE = re.compile(r'cubic-bezier\(\s*([-\d.]+)\s*,\s*([-\d.]+)\s*,\s*([-\d.]+)\s*,\s*([-\d.]+)\s*\)')


def match_css_easing_name(value: str) -> str | None:
    normalized = value.strip().lower()
    if normalized == "linear":
        return "LINEAR"
    if normalized in CSS_KEYWORD_BEZIERS:
        return match_bezier_preset(CSS_KEYWORD_BEZIERS[normalized])
    m = CUBIC_BEZIER_RE.match(normalized)
    if m:
        return match_bezier_preset(tuple(float(g) for g in m.groups()))
    return None  # unrecognized syntax (e.g. steps(...)) - not an error, just no signal


def detect_css_easing(bindings: list[CssBinding]) -> str | None:
    # Precedence 1: a per-step animation-timing-function overrides the shorthand's for
    # that segment under real CSS cascade semantics - if every step across every
    # binding declares an identical one, prefer it as the more specific signal.
    all_step_values = []
    any_step_declared = False
    for binding in bindings:
        for pct in binding.key_times:
            value = binding.step_timing_functions.get(pct)
            if value is not None:
                any_step_declared = True
            all_step_values.append(value)

    if any_step_declared:
        if len(set(all_step_values)) == 1 and all_step_values[0] is not None:
            return match_css_easing_name(all_step_values[0])
        return None  # some steps declared it and others didn't, or they conflict

    # Precedence 2: fall back to the shorthand's 3rd token, if every element agrees.
    shorthand_values = {b.shorthand_timing for b in bindings}
    if len(shorthand_values) == 1:
        (only,) = shorthand_values
        if only is not None:
            return match_css_easing_name(only)

    return None


def update_animated_icons_header(text: str, frame_identifiers: list[str], duration_seconds: float, getter_name: str,
                                  easing_name: str | None) -> str:
    for identifier in frame_identifiers:
        if f"::{identifier}" in text:
            raise RuntimeError(f"'{identifier}' is already referenced in {ANIMATED_ICONS_HEADER_PATH.name}")

    frames_initializer = ", ".join(f"NierikaDSPBinaryData::{ident}" for ident in frame_identifiers)
    # AnimatedIcon::easing is the struct's 3rd field, only included when detected - a
    # missing trailing aggregate member value-initializes to std::nullopt.
    easing_initializer = f", AnimationEasing::{easing_name}" if easing_name else ""
    new_line = (f"\n    static const AnimatedIcon {getter_name}() "
                f"{{ return {{ {{ {frames_initializer} }}, {duration_seconds}f{easing_initializer} }}; }}")
    return insert_before_class_close(text, "AnimatedIcons", new_line)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("svg_path", type=Path, help="Path to the animated .svg file to add")
    parser.add_argument("--name", help="Override the generated identifier's base name (e.g. 'Capture' -> CaptureFrame0_svg, ...)")
    parser.add_argument("--no-icon-getter", action="store_true", help="Don't add an accessor to include/gui/AnimatedIcons.h")
    parser.add_argument("--dry-run", action="store_true", help="Print what would change without writing any files")
    args = parser.parse_args()

    svg_path: Path = args.svg_path
    if svg_path.suffix.lower() != ".svg":
        print(f"error: '{svg_path}' is not a .svg file", file=sys.stderr)
        return 1
    if not svg_path.is_file():
        print(f"error: '{svg_path}' does not exist", file=sys.stderr)
        return 1

    raw_content = svg_path.read_text(encoding="utf-8")

    normalized_content, color_fixes = normalize_default_colors(raw_content)
    if color_fixes:
        print(f"note: rewrote {color_fixes} literal fill/stroke=\"black\" occurrence(s) to \"#000000\" "
              f"(required for theme-driven recoloring)")

    ET.register_namespace('', SVG_NS)
    if "xlink:" in normalized_content:
        ET.register_namespace('xlink', XLINK_NS)

    try:
        root = ET.fromstring(normalized_content)
    except ET.ParseError as e:
        print(f"error: '{svg_path}' is not well-formed XML: {e}", file=sys.stderr)
        return 1

    try:
        smil_bindings = collect_bindings(root)
        style_text = find_style_text(root)
        css_bindings = collect_css_bindings(root, style_text) if style_text.strip() else []

        if smil_bindings and css_bindings:
            raise SmilError("this SVG contains both SMIL <animate>/<animateTransform> tags and a CSS @keyframes "
                             "animation; mixing both mechanisms in one file is not supported - use one consistently")
        if not smil_bindings and not css_bindings:
            raise SmilError("no <animate>/<animateTransform> elements or CSS @keyframes animation found in this SVG")

        if smil_bindings:
            key_times, duration_seconds = validate_shared_schedule(smil_bindings)
            frame_count = len(key_times)
            frame_texts = snapshot_frames(root, smil_bindings, frame_count)
        else:
            key_times, duration_seconds = validate_shared_schedule(css_bindings)
            frame_count = len(key_times)
            frame_texts = snapshot_frames_css(root, css_bindings, frame_count)
    except SmilError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1

    # Easing detection is best-effort and must never block an otherwise-valid import.
    try:
        easing_name = detect_smil_easing(smil_bindings) if smil_bindings else detect_css_easing(css_bindings)
    except Exception:
        easing_name = None

    base_identifier = derive_identifier(svg_path, args.name, "")
    frame_identifiers = [f"{base_identifier}Frame{i}_svg" for i in range(frame_count)]

    header_text = HEADER_PATH.read_text(encoding="utf-8")
    cpp_text = CPP_PATH.read_text(encoding="utf-8")

    frame_sizes = []
    try:
        for i, (identifier, frame_text) in enumerate(zip(frame_identifiers, frame_texts)):
            size_bytes = len(frame_text.encode("utf-8"))
            frame_sizes.append(size_bytes)

            header_text = update_header(header_text, identifier, size_bytes)

            index = next_binary_data_index(cpp_text)
            escaped_lines = wrap_escaped(escape_c_string(frame_text))
            original_filename = f"{svg_path.stem}_frame{i}.svg"
            block = build_binary_data_block(index, original_filename, identifier, escaped_lines)
            cpp_text = update_cpp(cpp_text, identifier, original_filename, block, size_bytes)
    except RuntimeError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1

    new_animated_icons_text = None
    getter_name = None
    if not args.no_icon_getter:
        try:
            animated_icons_text = ANIMATED_ICONS_HEADER_PATH.read_text(encoding="utf-8")
        except FileNotFoundError:
            print(f"error: {ANIMATED_ICONS_HEADER_PATH.relative_to(REPO_ROOT)} does not exist - create it first",
                  file=sys.stderr)
            return 1
        getter_name = f"get{base_identifier[0].upper()}{base_identifier[1:]}"
        try:
            new_animated_icons_text = update_animated_icons_header(animated_icons_text, frame_identifiers,
                                                                     duration_seconds, getter_name, easing_name)
        except RuntimeError as e:
            print(f"error: {e}", file=sys.stderr)
            return 1

    print(f"identifier base: {base_identifier}")
    print(f"frames:          {frame_count}")
    print(f"duration:        {duration_seconds}s")
    print(f"easing:          {easing_name if easing_name else 'not detected (AnimatedSVG defaults to LINEAR)'}")
    for identifier, size_bytes in zip(frame_identifiers, frame_sizes):
        print(f"  {identifier}  (size {size_bytes} bytes, hash 0x{compute_projucer_hash(identifier):08x})")
    if getter_name:
        print(f"icon getter:     nierika::gui::AnimatedIcons::{getter_name}()")

    if args.dry_run:
        print("\n--dry-run: no files were modified")
        return 0

    HEADER_PATH.write_text(header_text, encoding="utf-8")
    CPP_PATH.write_text(cpp_text, encoding="utf-8")
    if new_animated_icons_text is not None:
        ANIMATED_ICONS_HEADER_PATH.write_text(new_animated_icons_text, encoding="utf-8")

    print(f"\nUpdated {HEADER_PATH.relative_to(REPO_ROOT)}, {CPP_PATH.relative_to(REPO_ROOT)}"
          + (f" and {ANIMATED_ICONS_HEADER_PATH.relative_to(REPO_ROOT)}" if new_animated_icons_text is not None else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
