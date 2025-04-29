#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class FrequencyDial: public Dial
{
public:
    FrequencyDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue);
    FrequencyDial(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~FrequencyDial() override;
    
    juce::String getTextFromValue(double value);

private:
    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencyDial)
};

}
