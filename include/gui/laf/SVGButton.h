#pragma once

namespace nierika::gui::element { class SVGButton; }

namespace nierika::gui::laf
{

class SVGButton final : public juce::LookAndFeel_V4
{
public:
    explicit SVGButton(const element::SVGButton& parent);
    ~SVGButton() override = default;

    void drawDrawableButton
    (
        juce::Graphics &g,
        juce::DrawableButton &button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown
    ) override;

private:
    const element::SVGButton& _parent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGButton)
};

}
