#include "../../include/gui/Helpers.h"

namespace nierika::gui::helpers
{

namespace
{
    void replaceDefaultColor(juce::XmlElement* element, const char* attributeName, const juce::String& colorHex)
    {
        if (juce::String value = element->getStringAttribute(attributeName); value.isNotEmpty())
        {
            value = value.replace(SVG_DEFAULT_COLOR, colorHex, true);
            element->setAttribute(attributeName, value);
        }
    }
}

void changeColor(const std::unique_ptr<juce::XmlElement>& xml, const juce::String& colorHex)
{
    for (const auto xmlNode : xml->getChildIterator())
    {
        replaceDefaultColor(xmlNode, ATTRIBUTE_FILL, colorHex);
        replaceDefaultColor(xmlNode, ATTRIBUTE_STROKE, colorHex);

        for (const auto xmlOuterDefs : xmlNode->getChildIterator())
        {
            replaceDefaultColor(xmlOuterDefs, ATTRIBUTE_FILL, colorHex);
            replaceDefaultColor(xmlOuterDefs, ATTRIBUTE_STROKE, colorHex);

            for (const auto xmlDefs : xmlOuterDefs->getChildIterator())
            {
                replaceDefaultColor(xmlDefs, ATTRIBUTE_FILL, colorHex);
                replaceDefaultColor(xmlDefs, ATTRIBUTE_STROKE, colorHex);
            }
        }
    }
}

namespace
{
    void drawFromXmlElement(juce::Graphics& g, const std::unique_ptr<juce::XmlElement>& svg, const juce::String& colHex,
                             int x, int y, int newWidth, int newHeight, juce::AffineTransform affine, const float opacity)
    {
        jassert(svg != nullptr);
        changeColor(svg, colHex);

        const std::unique_ptr<juce::Drawable> drawable = juce::Drawable::createFromSVG(*svg);

        const int side = juce::jmin(newWidth, newHeight);
        const int squaredX = x + (newWidth - side) / 2;
        const int squaredY = y + (newHeight - side) / 2;

        drawable->setTransformToFit(juce::Rectangle<float>(static_cast<float>(squaredX), static_cast<float>(squaredY),
                                                            static_cast<float>(side), static_cast<float>(side)),
                                     juce::RectanglePlacement::centred);
        drawable->draw(g, opacity, affine);
    }
}

void drawFromSVG(juce::Graphics& g, const char* svgBinary, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine, const float opacity)
{
    std::unique_ptr<juce::XmlElement> svg(juce::XmlDocument::parse(svgBinary));
    drawFromXmlElement(g, svg, colHex, x, y, newWidth, newHeight, affine, opacity);
}

void drawFromInterpolatedSVG(juce::Graphics& g, const juce::XmlElement& interpolatedFrame, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine, const float opacity)
{
    std::unique_ptr<juce::XmlElement> clone(new juce::XmlElement(interpolatedFrame));
    drawFromXmlElement(g, clone, colHex, x, y, newWidth, newHeight, affine, opacity);
}

void drawFromAnimatedSVG(juce::Graphics& g, const std::vector<const char*>& frames, const int frameIndex, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine)
{
    if (frames.empty()) return;

    drawFromSVG(g, frames[static_cast<std::size_t>(frameIndex) % frames.size()], colHex, x, y, newWidth, newHeight, affine);
}

void drawFromAnimatedSVGBlended(juce::Graphics& g, const std::vector<const char*>& frames, const int frameIndex, const int nextFrameIndex, const float blendAlpha, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine)
{
    if (frames.empty()) return;

    drawFromSVG(g, frames[static_cast<std::size_t>(frameIndex) % frames.size()], colHex, x, y, newWidth, newHeight, affine, 1.0f);

    const float alpha = juce::jlimit(0.f, 1.f, blendAlpha);
    if (alpha > 0.f)
        drawFromSVG(g, frames[static_cast<std::size_t>(nextFrameIndex) % frames.size()], colHex, x, y, newWidth, newHeight, affine, alpha);
}

}
