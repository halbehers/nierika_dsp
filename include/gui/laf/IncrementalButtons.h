#pragma once

namespace nierika::gui::laf
{

class IncrementalButtons final : public juce::LookAndFeel_V4
{
public:
    IncrementalButtons() = default;
    ~IncrementalButtons() override = default;
    
    juce::Button* createSliderButton(juce::Slider& slider, bool isIncrement) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IncrementalButtons)
};

}
