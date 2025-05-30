#pragma once

namespace nierika::gui::laf
{

class PitchSlider final : public juce::LookAndFeel_V4
{
public:
    PitchSlider() = default;
    ~PitchSlider() override = default;

    void drawLinearSlider(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        juce::Slider::SliderStyle style,
        juce::Slider &slider
    ) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchSlider)
};

}
