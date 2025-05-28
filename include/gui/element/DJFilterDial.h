#pragma once

#include "Dial.h"

namespace nierika::gui::element
{

class DJFilterDial: public Dial
{
public:
    DJFilterDial(const std::string& identifier, const std::string& label, float defaultValue = 0.f);
    DJFilterDial(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~DJFilterDial() override = default;
    
    static juce::String getTextFromValue(double value);

private:
    void setup() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DJFilterDial)
};

}
