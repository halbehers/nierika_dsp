#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class FrequencyDial: public Dial
{
public:
    FrequencyDial(const std::string& identifier, const juce::String& label, float minValue, float maxValue, float defaultValue, const Size size = MEDIUM);
    ~FrequencyDial() override;
    
    juce::String getTextFromValue(double value);
};

}
