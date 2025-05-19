#pragma once

namespace nierika::gui::element
{

class Dial: public Component
{
public:
    enum Size
    {
        SMALL,
        MEDIUM,
        LARGE
    };
    
    Dial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, const std::string& valueSuffix = "");
    Dial(dsp::ParameterManager& parameterManager, const std::string& parameterID, const std::string& valueSuffix = "");
    ~Dial() override;

    void resized() override;
    void setShortLabel(const juce::String& shortLabel);
    void setEnabled(bool isEnabled);
    
protected:
    float _minValue;
    float _maxValue;
    float _defaultValue;
    std::string _valueSuffix;

    juce::Slider _slider;
    laf::Dial _lookAndFeel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _attachment;

    virtual void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dial)
};

}
