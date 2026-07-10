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

void drawFromSVG(juce::Graphics& g, const char* svgBinary, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine)
{
    const std::unique_ptr<juce::XmlElement> svg(juce::XmlDocument::parse(svgBinary));
    jassert(svg != nullptr);
    changeColor(svg, colHex);

    const std::unique_ptr<juce::Drawable> drawable = juce::Drawable::createFromSVG(*svg);

    const int side = juce::jmin(newWidth, newHeight);
    const int squaredX = x + (newWidth - side) / 2;
    const int squaredY = y + (newHeight - side) / 2;

    drawable->setTransformToFit(juce::Rectangle<float>(static_cast<float>(squaredX), static_cast<float>(squaredY),
                                                        static_cast<float>(side), static_cast<float>(side)),
                                 juce::RectanglePlacement::centred);
    drawable->draw(g, 1.f, affine);
}

}
