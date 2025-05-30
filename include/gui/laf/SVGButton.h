#pragma once

namespace nierika::gui::laf
{

class SVGButton final : public juce::LookAndFeel_V4
{
public:
    SVGButton() = default;
    explicit SVGButton(const char* svgBinary);
    ~SVGButton() override = default;

    void drawDrawableButton
    (
        juce::Graphics &g,
        juce::DrawableButton &button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown
    ) override;

private:
    const char* _svgBinary;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGButton)
};

}
