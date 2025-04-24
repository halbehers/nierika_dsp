#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class TimeInMsDial: public Dial
{
public:
    TimeInMsDial(const std::string& identifier, const juce::String& label, float minValue, float maxValue, float defaultValue = 375, Size size = MEDIUM);
    ~TimeInMsDial() override;
    
    juce::String getTextFromValue(double value);
};

}
