#include "../../../include/gui/laf/TextButton.h"
#include "../../../include/gui/element/TextButton.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::laf
{

namespace
{
    // Mirrors the corner-based text indent JUCE's LookAndFeel_V2::drawButtonText uses, so an
    // icon (when present) lines up with where the text would otherwise start, and the
    // configurable gap only ever separates the icon from the text, not from the button edge.
    int cornerBasedIndent(int buttonWidth, int buttonHeight, int fontHeight, bool isConnected)
    {
        const int cornerSize = juce::jmin(buttonHeight, buttonWidth) / 2;
        return juce::jmin(fontHeight, 2 + cornerSize / (isConnected ? 4 : 2));
    }
}

TextButton::TextButton(const element::TextButton& parent):
    _parent(parent)
{
}

juce::Font TextButton::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    (void) button;
    return Theme::newFont(Theme::REGULAR, Theme::CAPTION);
}

void TextButton::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    (void) shouldDrawButtonAsDown;

    if (shouldDrawButtonAsHighlighted)
    {
        g.setColour(backgroundColour.withAlpha(0.9f));
        g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 8.f);
    }
    else if (!button.isEnabled())
    {
        g.setColour(backgroundColour.withAlpha(0.7f));
        g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 8.f);
    }
    else
    {
        g.setColour(backgroundColour);
        g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 8.f);
    }

    if (!_parent.hasIcon())
        return;

    auto& textButton = static_cast<juce::TextButton&>(button);
    const bool isLeft = _parent.getIconPosition() == element::TextButton::IconPosition::LEFT;
    const int fontHeight = juce::roundToInt(getTextButtonFont(textButton, button.getHeight()).getHeight() * 0.6f);
    const int edgeIndent = cornerBasedIndent(button.getWidth(), button.getHeight(), fontHeight,
                                              isLeft ? textButton.isConnectedOnLeft() : textButton.isConnectedOnRight());

    const float iconSize = _parent.getIconSize();
    const auto bounds = button.getLocalBounds().toFloat();
    const float iconY = (bounds.getHeight() - iconSize) * 0.5f;
    const float iconX = isLeft ? static_cast<float>(edgeIndent) : bounds.getWidth() - static_cast<float>(edgeIndent) - iconSize;

    helpers::drawFromSVG(g, _parent.getIconBinary(), Theme::newColor(_parent.hasInvertedTextColor() ? Theme::ThemeColor::INVERTED_TEXT : Theme::ThemeColor::TEXT).asHexString(),
                          static_cast<int>(iconX), static_cast<int>(iconY),
                          static_cast<int>(iconSize), static_cast<int>(iconSize),
                          juce::AffineTransform());
}

void TextButton::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    (void) shouldDrawButtonAsHighlighted;
    (void) shouldDrawButtonAsDown;

    const juce::Font font(getTextButtonFont(button, button.getHeight()));
    g.setFont(font);
    g.setColour(Theme::newColor(_parent.hasInvertedTextColor() ? Theme::ThemeColor::INVERTED_TEXT : Theme::ThemeColor::TEXT).asJuce().withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

    const int yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    const int fontHeight = juce::roundToInt(font.getHeight() * 0.6f);

    int leftIndent = cornerBasedIndent(button.getWidth(), button.getHeight(), fontHeight, button.isConnectedOnLeft());
    int rightIndent = cornerBasedIndent(button.getWidth(), button.getHeight(), fontHeight, button.isConnectedOnRight());

    if (_parent.hasIcon())
    {
        // The icon sits at the same base indent the text would otherwise start at (see
        // drawButtonBackground), so the gap here only needs to cover icon size + spacing to text.
        const int iconAndGap = juce::roundToInt(_parent.getIconSize() + _parent.getGap());
        if (_parent.getIconPosition() == element::TextButton::IconPosition::LEFT)
            leftIndent += iconAndGap;
        else
            rightIndent += iconAndGap;
    }

    const int textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
                          leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                          juce::Justification::centred, 2);
}

}
