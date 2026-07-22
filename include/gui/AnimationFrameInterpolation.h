#pragma once

#include <memory>
#include <optional>

namespace juce { class XmlElement; }

namespace nierika::gui::animation
{

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

[[nodiscard]] std::optional<Affine2DDecomposed> decompose2x2(float a, float b, float c, float d);

[[nodiscard]] Matrix2x2 recompose2x2(const Affine2DDecomposed& decomposed);

[[nodiscard]] Affine2DDecomposed interpolateDecomposed(const Affine2DDecomposed& a, const Affine2DDecomposed& b, float t);

[[nodiscard]] std::unique_ptr<juce::XmlElement> interpolateSVGFrame(const char* frameA, const char* frameB, float t);

}
