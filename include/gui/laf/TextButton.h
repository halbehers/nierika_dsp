#pragma once

#include "../Theme.h"

namespace nierika::gui::laf
{

class TextButton : public juce::LookAndFeel_V4
{
public:
    TextButton();
    ~TextButton() override;

    juce::Font getTextButtonFont(juce::TextButton& button, int buttonHeight) override;
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextButton)
};

}
