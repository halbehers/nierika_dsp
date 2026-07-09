#include "../../../include/gui/laf/PrimaryTextButton.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

juce::Font PrimaryTextButton::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    (void) button;
    return Theme::newFont(Theme::REGULAR).withHeight(static_cast<float>(buttonHeight) * 0.65f);
}

void PrimaryTextButton::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
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
}

}
