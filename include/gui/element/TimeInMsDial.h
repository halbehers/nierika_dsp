#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class TimeInMsDial: public Dial
{
public:
    TimeInMsDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue = 375, Size size = MEDIUM);
    TimeInMsDial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, Size size = Size::MEDIUM);
    ~TimeInMsDial() override;
    
    juce::String getTextFromValue(double value);

private:
    void setup();
};

}
