#include "../../../include/gui/laf/SVGToggle.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::laf
{

SVGToggle::SVGToggle(const element::SVGToggle& parent):
    _parent(parent)
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
    if (_parent.getIconBinary() == nullptr)
    {
        juce::LookAndFeel_V4::drawTickBox(g, button, x, y, w, h, ticked, isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        return;
    }

    const Theme::ThemeColor themeColor = _stateToThemeColors[getState(ticked, isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown)];

    const std::string color = Theme::newColor(themeColor).asHexString();

    const float iconSize = _parent.getIconSize();
    const float ix = (static_cast<float>(button.getWidth()) - iconSize) * 0.5f;
    const float iy = (static_cast<float>(button.getHeight()) - iconSize) * 0.5f;

    helpers::drawFromSVG(g, _parent.getIconBinary(), color,
                          static_cast<int>(ix), static_cast<int>(iy),
                          static_cast<int>(iconSize), static_cast<int>(iconSize),
                          juce::AffineTransform());
}

void SVGToggle::drawToggleButton
(
    juce::Graphics& g,
    juce::ToggleButton& button,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown
)
{
    if (_parent.getIconBinary() == nullptr)
    {
        juce::LookAndFeel_V4::drawToggleButton(g, button, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
        return;
    }

    // Mirrors LookAndFeel_V4::drawToggleButton's tick sizing, but centres the tick box
    // within the full button bounds instead of pinning it to a fixed left offset - that
    // default assumes a checkbox-with-label layout, which doesn't apply to an icon-only toggle.
    const auto fontSize = juce::jmin(15.0f, static_cast<float>(button.getHeight()) * 0.75f);
    const auto tickWidth = fontSize * 1.1f;

    drawTickBox(g, button, (static_cast<float>(button.getWidth()) - tickWidth) * 0.5f,
                (static_cast<float>(button.getHeight()) - tickWidth) * 0.5f,
                tickWidth, tickWidth,
                button.getToggleState(),
                button.isEnabled(),
                shouldDrawButtonAsHighlighted,
                shouldDrawButtonAsDown);

    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.setFont(fontSize);

    if (!button.isEnabled())
        g.setOpacity(0.5f);

    g.drawFittedText(button.getButtonText(),
                      button.getLocalBounds().withTrimmedLeft(juce::roundToInt(tickWidth) + 10).withTrimmedRight(2),
                      juce::Justification::centredLeft, 10);
}

}
