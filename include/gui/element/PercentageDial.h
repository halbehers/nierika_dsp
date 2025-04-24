#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class PercentageDial: public Dial
{
public:
    PercentageDial(const std::string& identifier, const juce::String& label, float minValue, float maxValue, float defaultValue = 0.5, Size size = MEDIUM);
    ~PercentageDial() override;
    
    juce::String getTextFromValue(double value);
};

}
