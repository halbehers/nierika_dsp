#include "../../include/gui/Helpers.h"

namespace nierika::gui::helpers
{

void changeColor(const std::unique_ptr<juce::XmlElement>& xml, const juce::String& colorHex)
{
    for (const auto xmlNode : xml->getChildIterator())
    {
        if (juce::String fill = xmlNode->getStringAttribute(ATTRIBUTE_FILL); fill.isNotEmpty())
        {
            fill = fill.replace(SVG_DEFAULT_COLOR, colorHex, true);
            xmlNode->setAttribute(ATTRIBUTE_FILL, fill);
        }
        for (const auto xmlOuterDefs : xmlNode->getChildIterator())
        {
            if (juce::String fill = xmlOuterDefs->getStringAttribute(ATTRIBUTE_FILL); fill.isNotEmpty())
            {
                fill = fill.replace(SVG_DEFAULT_COLOR, colorHex, true);
                xmlOuterDefs->setAttribute(ATTRIBUTE_FILL, fill);
            }

            for (const auto xmlDefs : xmlOuterDefs->getChildIterator())
            {
                if (juce::String fill = xmlDefs->getStringAttribute(ATTRIBUTE_FILL); fill.isNotEmpty())
                {
                    fill = fill.replace(SVG_DEFAULT_COLOR, colorHex, true);
                    xmlDefs->setAttribute(ATTRIBUTE_FILL, fill);
                }
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
    drawable->setTransformToFit(juce::Rectangle<float>(x, y, newWidth, newHeight), juce::RectanglePlacement::centred);
        drawable->draw(g, 1.f, affine);
}

}
