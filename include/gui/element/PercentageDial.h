#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class PercentageDial: public Dial
{
public:
    PercentageDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue = 0.5);
    PercentageDial(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~PercentageDial() override;

    juce::String getTextFromValue(double value);

private:
    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PercentageDial)
};

}
