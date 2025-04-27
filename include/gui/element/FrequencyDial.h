#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class FrequencyDial: public Dial
{
public:
    FrequencyDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, const Size size = MEDIUM);
    FrequencyDial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, Size size = Size::MEDIUM);
    ~FrequencyDial() override;
    
    juce::String getTextFromValue(double value);

private:
    void setup();
};

}
