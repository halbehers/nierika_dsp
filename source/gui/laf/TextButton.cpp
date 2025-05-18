#include "../../../include/gui/laf/TextButton.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

juce::Font TextButton::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    return Theme::getInstance().getFont(Theme::LIGHT, Theme::CAPTION).withHeight(buttonHeight * 0.75f);       
}

void TextButton::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (shouldDrawButtonAsHighlighted)
    {
        g.setColour(backgroundColour.withAlpha(0.2f));
        g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 8.f);
    }
    else if (!button.isEnabled())
    {
        g.setColour(backgroundColour.withAlpha(0.1f));
        g.fillRoundedRectangle(button.getLocalBounds().toFloat(), 8.f);
    }
}

}
