#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class PercentageDial: public Dial
{
public:
    PercentageDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue = 0.5, Size size = MEDIUM);
    PercentageDial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, Size size = Size::MEDIUM);
    ~PercentageDial() override;
    
    juce::String getTextFromValue(double value);

private:
    void setup();
};

}
