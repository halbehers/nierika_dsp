#pragma once

namespace nierika::ui::element
{

class Slider: public juce::Slider
{
    public:
    Slider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix);
    ~Slider() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void setEnabled(bool isEnabled);
    
    protected:
    juce::Label _labelComponent;
};

}
