#pragma once

namespace nierika::gui::element
{

class HorizontalSlider: public Slider
{
public:
    HorizontalSlider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix);
    ~HorizontalSlider() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HorizontalSlider)
};

}
