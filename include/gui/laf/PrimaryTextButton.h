#pragma once

namespace nierika::gui::laf
{

class PrimaryTextButton final : public juce::LookAndFeel_V4
{
public:
    PrimaryTextButton() = default;
    ~PrimaryTextButton() override = default;

    juce::Font getTextButtonFont(juce::TextButton& button, int buttonHeight) override;
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrimaryTextButton)
};

}
