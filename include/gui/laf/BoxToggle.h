#pragma once

namespace nierika::gui::laf
{

class BoxToggle final : public juce::LookAndFeel_V4
{
public:
    BoxToggle() = default;
    ~BoxToggle() override = default;

    void drawTickBox
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
     ) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoxToggle)
};

}
