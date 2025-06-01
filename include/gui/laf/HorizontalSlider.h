#pragma once

namespace nierika::gui::laf
{

class HorizontalSlider : public juce::LookAndFeel_V4
{
public:
    HorizontalSlider() = default;
    ~HorizontalSlider() override = default;

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

    static int computeThumbX(int x, int width, float sliderPos, int thumbSize);

    static int computeThumbSize(int height);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HorizontalSlider)
};

}
