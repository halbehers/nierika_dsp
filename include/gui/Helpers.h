#pragma once

#define ATTRIBUTE_FILL "fill"
#define ATTRIBUTE_STROKE "stroke"
#define SVG_DEFAULT_COLOR "#000000"

namespace nierika::gui::helpers
{
void changeColor(const std::unique_ptr<juce::XmlElement>& xml, const juce::String& colorHex);

void drawFromSVG(juce::Graphics& g, const char* svgBinary, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine);

enum ClickableSurface {
    ALL_AVAILABLE_AREA,
    ELEMENT_BOUNDARIES
};


}
