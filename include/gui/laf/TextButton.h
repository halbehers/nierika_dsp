#pragma once

namespace nierika::gui::element { class TextButton; }

namespace nierika::gui::laf
{

class TextButton final : public juce::LookAndFeel_V4
{
public:
    explicit TextButton(const element::TextButton& parent);
    ~TextButton() override = default;

    juce::Font getTextButtonFont(juce::TextButton& button, int buttonHeight) override;
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

protected:
    const element::TextButton& _parent;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextButton)
};

}
