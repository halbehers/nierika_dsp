#pragma once

namespace nierika::gui::element
{

class TimeInMsDial: public Dial
{
public:
    TimeInMsDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue = 375);
    TimeInMsDial(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~TimeInMsDial() override = default;
    
    static juce::String getTextFromValue(double value);

private:
    void setup() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeInMsDial)
};

}
