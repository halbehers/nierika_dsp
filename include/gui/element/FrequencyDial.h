#pragma once

#include "Dial.h"

namespace nierika::ui::element
{

class FrequencyDial: public Dial
{
public:
    FrequencyDial(const juce::String& label, float minValue, float maxValue, float defaultValue, const Size size = MEDIUM);
    ~FrequencyDial() override;
    
    juce::String getTextFromValue(double value) override;
};

}
