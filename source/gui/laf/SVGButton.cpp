#include "../../../includes/gui/laf/SVGButton.h"
#include "../../../includes/gui/Theme.h"
#include "../../../includes/gui/Helpers.h"
#include "../../../includes/gui/EmbeddedFonts.h"

namespace ui::laf
{

SVGButton::SVGButton()
{
}

SVGButton::SVGButton(const char* svgBinary)
{
    _svgBinary = svgBinary;
}

SVGButton::~SVGButton()
{
}


void SVGButton::drawDrawableButton
(
    juce::Graphics &g,
    juce::DrawableButton &button,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown
)
{
    auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asHexString();
    auto highlightedColor = Theme::getInstance().getColor(Theme::ThemeColor::LIGHT_SHADE).asHexString();
    auto downColor = Theme::getInstance().getColor(Theme::ThemeColor::MEDIUM_SHADE).asHexString();
    auto disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED).asHexString();
    
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
    
    ui::helpers::drawFromSVG(g, _svgBinary, color, 0, 0, button.getWidth(), button.getHeight(), juce::AffineTransform());
}

}
