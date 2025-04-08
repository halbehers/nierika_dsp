#include "../../include/gui/Helpers.h"

namespace nierika::gui::helpers
{

void changeColor(std::unique_ptr<juce::XmlElement>& xml, juce::String color_hex)
{
    for (auto xmlnode : xml->getChildIterator())
    {
        juce::String fill = xmlnode->getStringAttribute(ATTRIBUTE_FILL);
        if (fill.isNotEmpty())
        {
            fill = fill.replace(SVG_DEFAULT_COLOR, color_hex, true);
            xmlnode->setAttribute(ATTRIBUTE_FILL, fill);
        }
        for (auto xmlouterdefs : xmlnode->getChildIterator())
        {
            juce::String fill = xmlouterdefs->getStringAttribute(ATTRIBUTE_FILL);
            if (fill.isNotEmpty())
            {
                fill = fill.replace(SVG_DEFAULT_COLOR, color_hex, true);
                xmlouterdefs->setAttribute(ATTRIBUTE_FILL, fill);
            }

            for (auto xmldefs : xmlouterdefs->getChildIterator())
            {
                juce::String fill = xmldefs->getStringAttribute(ATTRIBUTE_FILL);
                if (fill.isNotEmpty())
                {
                    fill = fill.replace(SVG_DEFAULT_COLOR, color_hex, true);
                    xmldefs->setAttribute(ATTRIBUTE_FILL, fill);
                }
            }
        }
    }
}

void drawFromSVG(juce::Graphics& g, const char* svgBinary, juce::String col_hex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine)
{
    std::unique_ptr<juce::XmlElement> svg(juce::XmlDocument::parse(svgBinary));
    jassert(svg != nullptr);
    changeColor(svg, col_hex);

    std::unique_ptr<juce::Drawable> drawable = juce::Drawable::createFromSVG(*svg);
    drawable->setTransformToFit(juce::Rectangle<float>(x, y, newWidth, newHeight), juce::RectanglePlacement::centred);
        drawable->draw(g, 1.f, affine);
}

}
