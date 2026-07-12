#include "../../../include/gui/laf/SVGButton.h"
#include "../../../include/gui/element/SVGButton.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::laf
{

SVGButton::SVGButton(const element::SVGButton& parent):
    _parent(parent)
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

    const float iconSize = _parent.getIconSize();
    const float x = (static_cast<float>(button.getWidth()) - iconSize) * 0.5f;
    const float y = (static_cast<float>(button.getHeight()) - iconSize) * 0.5f;

    helpers::drawFromSVG(g, _parent.getIconBinary(), color,
                          static_cast<int>(x), static_cast<int>(y),
                          static_cast<int>(iconSize), static_cast<int>(iconSize),
                          juce::AffineTransform());
}

}
