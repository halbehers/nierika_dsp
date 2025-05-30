#pragma once

namespace nierika::gui::laf
{

class Dial final : public juce::LookAndFeel_V4
{
public:
    enum LabelVisibility
    {
        VISIBLE,
        LABEL_ONLY,
        VALUE_ONLY,
        HIDDEN
    };

    Dial() = default;
    ~Dial() override = default;

    void drawRotarySlider
    (
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider &slider
    ) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    void setShortLabel(const juce::String& shortLabel);

    void setLabelVisibility(LabelVisibility visibility);

private:
    float _sliderWidth = 0.f;
    bool sliderIsDial = true;
    bool isReversed = false;
    juce::String _sliderName = "";
    juce::String _shortLabel = "";
    LabelVisibility _labelVisibility = VISIBLE;
    const Theme::FontSize _labelHeight = Theme::HEADING;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dial)
};

}
