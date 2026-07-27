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
    const std::string color = helpers::toHexString(_parent.getColour());
    const float initialColorOpacity = .8f;
    const float highlightedColorOpacity = 1.f;
    const float downColorOpacity = .7f;
    const float disabledColorOpacity = .5f;

    float opacity = initialColorOpacity;

    if (!button.isEnabled())
    {
        opacity = disabledColorOpacity;
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        opacity = highlightedColorOpacity;
    }
    else if (shouldDrawButtonAsDown)
    {
        opacity = downColorOpacity;
    }

    const float iconSize = _parent.getIconSize();
    const float x = (static_cast<float>(button.getWidth()) - iconSize) * 0.5f;
    const float y = (static_cast<float>(button.getHeight()) - iconSize) * 0.5f;

    helpers::drawFromSVG(g, _parent.getIconBinary(), color,
                          static_cast<int>(x), static_cast<int>(y),
                          static_cast<int>(iconSize), static_cast<int>(iconSize),
                          juce::AffineTransform(), opacity, _parent.getBackgroundColour());
}

}
