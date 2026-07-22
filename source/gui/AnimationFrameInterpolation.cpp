#include "../../include/gui/AnimationFrameInterpolation.h"

#include <cmath>
#include <cstdlib>
#include <utility>
#include <vector>

namespace nierika::gui::animation
{

std::optional<Affine2DDecomposed> decompose2x2(const float a, const float b, const float c, const float d)
{
    const float scaleXRaw = std::sqrt(a * a + b * b);
    if (scaleXRaw < 1e-6f) return std::nullopt;

    float ux = a / scaleXRaw, uy = b / scaleXRaw;
    float scaleX = scaleXRaw;

    float skew = ux * c + uy * d;
    const float c1 = c - ux * skew;
    const float d1 = d - uy * skew;

    const float scaleY = std::sqrt(c1 * c1 + d1 * d1);
    if (scaleY < 1e-6f) return std::nullopt;

    skew /= scaleY;

    if (a * d - b * c < 0.f)
    {
        ux = -ux; uy = -uy;
        skew = -skew;
        scaleX = -scaleX;
    }

    Affine2DDecomposed result;
    result.rotation = std::atan2(uy, ux);
    result.skewAngle = std::atan(skew);
    result.scaleX = scaleX;
    result.scaleY = scaleY;
    return result;
}

Matrix2x2 recompose2x2(const Affine2DDecomposed& decomposed)
{
    const float cosR = std::cos(decomposed.rotation);
    const float sinR = std::sin(decomposed.rotation);
    const float tanSkew = std::tan(decomposed.skewAngle);

    Matrix2x2 m;
    m.a = decomposed.scaleX * cosR;
    m.b = decomposed.scaleX * sinR;
    m.c = decomposed.scaleY * (tanSkew * cosR - sinR);
    m.d = decomposed.scaleY * (tanSkew * sinR + cosR);
    return m;
}

namespace
{
    float lerp(const float a, const float b, const float t) { return a + (b - a) * t; }

    float lerpAngleShortestPath(const float a, const float b, const float t)
    {
        constexpr float twoPi = juce::MathConstants<float>::twoPi;
        float delta = b - a;
        delta -= twoPi * std::round(delta / twoPi);
        return a + delta * t;
    }
}

Affine2DDecomposed interpolateDecomposed(const Affine2DDecomposed& a, const Affine2DDecomposed& b, const float t)
{
    Affine2DDecomposed result;
    result.rotation = lerpAngleShortestPath(a.rotation, b.rotation, t);
    result.skewAngle = lerp(a.skewAngle, b.skewAngle, t);
    result.scaleX = lerp(a.scaleX, b.scaleX, t);
    result.scaleY = lerp(a.scaleY, b.scaleY, t);
    return result;
}

namespace
{
    // Strict parse: the whole (trimmed) string must be a single valid float, not just a
    // leading numeric prefix - juce::String::getFloatValue() silently returns 0 for
    // garbage input, which would misreport an incompatible attribute as "compatible, but 0".
    bool tryParseFloatStrict(const juce::String& text, float& outValue)
    {
        const juce::String trimmed = text.trim();
        if (trimmed.isEmpty()) return false;

        const auto utf8 = trimmed.toRawUTF8();
        char* end = nullptr;
        const double parsed = std::strtod(utf8, &end);
        if (end == utf8 || *end != '\0') return false;

        outValue = static_cast<float>(parsed);
        return true;
    }

    // Parses "matrix(a b c d e f)" (space or comma separated, matching both this
    // module's own output and the snapshot script's formatting convention).
    bool tryParseMatrix(const juce::String& text, float (&out)[6])
    {
        const juce::String trimmed = text.trim();
        if (!trimmed.startsWithIgnoreCase("matrix(") || !trimmed.endsWithChar(')')) return false;

        const juce::String inner = trimmed.substring(7, trimmed.length() - 1);
        juce::StringArray tokens;
        tokens.addTokens(inner, " ,", "");
        tokens.removeEmptyStrings();
        if (tokens.size() != 6) return false;

        for (int i = 0; i < 6; ++i)
            if (!tryParseFloatStrict(tokens[i], out[i]))
                return false;

        return true;
    }

    juce::String formatMatrix(const float (&m)[6])
    {
        juce::String result("matrix(");
        for (int i = 0; i < 6; ++i)
        {
            if (i > 0) result << " ";
            result << juce::String(m[i], 4);
        }
        result << ")";
        return result;
    }

    // Returns false (incompatible) if valueA/valueB can't be interpolated together.
    bool interpolateAttributeValue(const juce::String& attributeName, const juce::String& valueA,
                                    const juce::String& valueB, const float t, juce::String& outValue)
    {
        if (attributeName == "transform")
        {
            float ma[6], mb[6];
            if (!tryParseMatrix(valueA, ma) || !tryParseMatrix(valueB, mb)) return false;

            const auto decomposedA = decompose2x2(ma[0], ma[1], ma[2], ma[3]);
            const auto decomposedB = decompose2x2(mb[0], mb[1], mb[2], mb[3]);
            if (!decomposedA.has_value() || !decomposedB.has_value()) return false;

            const auto interpolated = interpolateDecomposed(*decomposedA, *decomposedB, t);
            const auto recomposed = recompose2x2(interpolated);

            const float tx = lerp(ma[4], mb[4], t);
            const float ty = lerp(ma[5], mb[5], t);

            const float out[6] = { recomposed.a, recomposed.b, recomposed.c, recomposed.d, tx, ty };
            outValue = formatMatrix(out);
            return true;
        }

        float a, b;
        if (!tryParseFloatStrict(valueA, a) || !tryParseFloatStrict(valueB, b)) return false;

        outValue = juce::String(lerp(a, b, t), 4);
        return true;
    }

    bool interpolateElement(juce::XmlElement& cloneElem, const juce::XmlElement& otherElem, const float t)
    {
        if (cloneElem.getTagName() != otherElem.getTagName()) return false;

        std::vector<std::pair<juce::String, juce::String>> updates;
        int numAttributes = 0;

        for (const auto& attr : cloneElem.getAttributeIterator())
        {
            ++numAttributes;
            const juce::String name = attr.name.toString();

            if (!otherElem.hasAttribute(name)) return false;

            const juce::String otherValue = otherElem.getStringAttribute(name);
            if (attr.value == otherValue) continue;

            juce::String interpolated;
            if (!interpolateAttributeValue(name, attr.value, otherValue, t, interpolated)) return false;

            updates.emplace_back(name, interpolated);
        }

        int otherAttributeCount = 0;
        for (const auto& attr : otherElem.getAttributeIterator()) { (void) attr; ++otherAttributeCount; }
        if (otherAttributeCount != numAttributes) return false;

        for (const auto& [name, value] : updates)
            cloneElem.setAttribute(name, value);

        if (cloneElem.getNumChildElements() != otherElem.getNumChildElements()) return false;

        auto* cloneChild = cloneElem.getFirstChildElement();
        auto* otherChild = otherElem.getFirstChildElement();
        while (cloneChild != nullptr && otherChild != nullptr)
        {
            if (!interpolateElement(*cloneChild, *otherChild, t)) return false;

            cloneChild = cloneChild->getNextElement();
            otherChild = otherChild->getNextElement();
        }

        return cloneChild == nullptr && otherChild == nullptr;
    }
}

std::unique_ptr<juce::XmlElement> interpolateSVGFrame(const char* frameA, const char* frameB, const float t)
{
    const std::unique_ptr<juce::XmlElement> docA(juce::XmlDocument::parse(frameA));
    const std::unique_ptr<juce::XmlElement> docB(juce::XmlDocument::parse(frameB));
    if (docA == nullptr || docB == nullptr) return nullptr;

    auto clone = std::make_unique<juce::XmlElement>(*docA);

    if (!interpolateElement(*clone, *docB, t)) return nullptr;

    return clone;
}

}
