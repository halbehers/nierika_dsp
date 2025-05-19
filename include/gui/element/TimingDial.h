#pragma once

#include "Dial.h"
#include "../../dsp/Timing.h"

namespace nierika::gui::element
{

class TimingDial: public Dial
{
public:
    TimingDial(const std::string& identifier, const std::string& label, dsp::Timing::NoteTiming defaultValue);
    TimingDial(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~TimingDial() override = default;

    juce::String getTextFromValue(double value);

private:
    const std::unordered_map<dsp::Timing::NoteTiming, std::string> _timingToNames = dsp::Timing::getNoteTimingToNames();
    void setup() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimingDial)
};

}
