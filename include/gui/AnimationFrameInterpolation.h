#pragma once

#include <memory>
#include <optional>

namespace juce { class XmlElement; }

// Genuine geometric interpolation between two pre-baked AnimatedSVG frames, as opposed
// to the opacity crossfade in Helpers.h/.cpp. Kept in its own file (rather than
// AnimationEasing.h) so the pure easing/timing math stays free of juce::XmlElement
// dependencies; the matrix math here is itself pure/XML-free for the same reason -
// only interpolateSVGFrame() touches XML.
namespace nierika::gui::animation
{

// A 2D affine matrix's linear part (a,b,c,d in SVG's matrix(a b c d e f)) decomposed
// into independently-interpolatable components. Translation (e,f) is handled
// separately by the caller via a plain lerp - it's not part of this decomposition.
struct Affine2DDecomposed
{
    float rotation = 0.f;   // radians
    float skewAngle = 0.f;  // radians
    float scaleX = 1.f;
    float scaleY = 1.f;
};

struct Matrix2x2
{
    float a = 1.f, b = 0.f, c = 0.f, d = 1.f;
};

// Returns std::nullopt for a degenerate (near-zero-scale) matrix rather than risking NaN.
[[nodiscard]] std::optional<Affine2DDecomposed> decompose2x2(float a, float b, float c, float d);

[[nodiscard]] Matrix2x2 recompose2x2(const Affine2DDecomposed& decomposed);

// Component-wise interpolation, t in [0,1]. Rotation uses shortest-path interpolation.
[[nodiscard]] Affine2DDecomposed interpolateDecomposed(const Affine2DDecomposed& a, const Affine2DDecomposed& b, float t);

// Generates a genuinely interpolated in-between SVG frame at progress t between two
// complete, pre-baked frame documents. frameA/frameB must be structurally isomorphic
// (guaranteed for any two frames of the same AnimatedIcon, since the snapshot script
// builds every frame from the same source tree with only specific attributes mutated).
// Returns nullptr if that assumption doesn't hold for some reason (tag/child-count/
// attribute-set mismatch, or a differing attribute that's neither "transform" nor a
// plain parseable number) - the caller should fall back to an opacity crossfade
// between the two original frames in that case.
[[nodiscard]] std::unique_ptr<juce::XmlElement> interpolateSVGFrame(const char* frameA, const char* frameB, float t);

}
