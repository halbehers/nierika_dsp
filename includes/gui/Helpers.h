#pragma once

#define ATTRIBUTE_FILL "fill"
#define SVG_DEFAULT_COLOR "#000000"

namespace nierika::ui::helpers
{

void changeColor(juce::XmlElement* xml, juce::String color_hex);

void drawFromSVG(juce::Graphics& g, const char* svgBinary, juce::String col_hex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine);

}
