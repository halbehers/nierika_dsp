#include "../../../include/gui/laf/SVGButton.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::laf
{

SVGButton::SVGButton(const char* svgBinary)
{
    _svgBinary = svgBinary;
}

void SVGButton::drawDrawableButton
(
    juce::Graphics &g,
    juce::DrawableButton &button,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown
)
{
    const std::string whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asHexString();
    const std::string highlightedColor = Theme::newColor(Theme::ThemeColor::LIGHT_SHADE).asHexString();
    const std::string downColor = Theme::newColor(Theme::ThemeColor::MEDIUM_SHADE).asHexString();
    const std::string disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED).asHexString();
    
    auto color = whiteColor;
    
    if (!button.isEnabled())
    {
        color = disabledColor;
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        color = highlightedColor;
    }
    else if (shouldDrawButtonAsDown)
    {
        color = downColor;
    }
    
    helpers::drawFromSVG(g, _svgBinary, color, 0, 0, button.getWidth(), button.getHeight(), juce::AffineTransform());
}

}
