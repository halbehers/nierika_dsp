#include "../../../include/gui/laf/SVGToggle.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"
#include "../../../include/gui/EmbeddedFonts.h"

namespace ui::laf
{

SVGToggle::SVGToggle()
{
}

SVGToggle::SVGToggle(const char* svgBinary)
{
    _svgBinary = svgBinary;
}

SVGToggle::~SVGToggle()
{
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

    auto themeColor = _stateToThemeColors[getState(ticked, isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown)];
    auto color = Theme::getInstance().getColor(themeColor).asHexString();

    ui::helpers::drawFromSVG(g, _svgBinary, color, x, y, w, h, juce::AffineTransform());
    
    
    
}

}
