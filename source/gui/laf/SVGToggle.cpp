#include "../../../include/gui/laf/SVGToggle.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::laf
{

SVGToggle::SVGToggle(const char* svgBinary)
{
    _svgBinary = svgBinary;
}

SVGToggle::State SVGToggle::getState(bool ticked,
                                bool isEnabled,
                                bool shouldDrawButtonAsHighlighted,
                                bool shouldDrawButtonAsDown)
{
    if (!isEnabled) return SVGToggle::State::DISABLED;
    if (shouldDrawButtonAsDown) return SVGToggle::State::DOWN;
    if (shouldDrawButtonAsHighlighted) return SVGToggle::State::HIGHLIGHTED;
    if (ticked) return SVGToggle::State::SELECTED;

    return SVGToggle::State::DEFAULT;
}

void SVGToggle::drawTickBox
(
    juce::Graphics& g,
    juce::Component& button,
    float x,
    float y,
    float w,
    float h,
    bool ticked,
    bool isEnabled,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown
)
{
    if (_svgBinary == nullptr)
    {
        juce::LookAndFeel_V4::drawTickBox(g, button, x, y, w, h, ticked, isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        return;
    }

    const Theme::ThemeColor themeColor = _stateToThemeColors[getState(ticked, isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown)];

    const std::string color = Theme::newColor(themeColor).asHexString();

    helpers::drawFromSVG(g, _svgBinary, color, static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h), juce::AffineTransform());
}

}
