#pragma once

#include <vector>

#define ATTRIBUTE_FILL "fill"
#define ATTRIBUTE_STROKE "stroke"
#define SVG_DEFAULT_COLOR "#000000"

namespace nierika::gui::helpers
{
void changeColor(const std::unique_ptr<juce::XmlElement>& xml, const juce::String& colorHex);

void drawFromSVG(juce::Graphics& g, const char* svgBinary, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine, float opacity = 1.0f);

void drawFromAnimatedSVG(juce::Graphics& g, const std::vector<const char*>& frames, int frameIndex, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine);

// Draws frames[frameIndex] fully opaque as a base, then frames[nextFrameIndex] layered on
// top at blendAlpha (0 = only the base frame is visible, 1 = only the next frame is) - a
// simple two-layer crossfade dissolve. No AnimationEasing here - callers resolve easing
// into a plain blendAlpha upstream (see animation::computeAnimationFrameBlend).
void drawFromAnimatedSVGBlended(juce::Graphics& g, const std::vector<const char*>& frames, int frameIndex, int nextFrameIndex, float blendAlpha, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine);

// Draws an already-generated (e.g. cached) geometrically-interpolated frame tree -
// see animation::interpolateSVGFrame. Clones it internally before recoloring mutates
// attributes in place, so the caller's cached tree is never touched.
void drawFromInterpolatedSVG(juce::Graphics& g, const juce::XmlElement& interpolatedFrame, const juce::String& colHex, int x, int y, int newWidth, int newHeight, juce::AffineTransform affine, float opacity = 1.0f);

enum ClickableSurface {
    ALL_AVAILABLE_AREA,
    ELEMENT_BOUNDARIES
};


}
