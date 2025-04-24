#pragma once

#include "Dial.h"
#include "../../dsp/Timing.h"

namespace nierika::gui::element
{

class TimingDial: public Dial
{
public:
    TimingDial(const std::string& identifier, const juce::String& label, dsp::Timing::NoteTiming defaultValue, Size size = MEDIUM);
    ~TimingDial() override;

    juce::String getTextFromValue(double value);

private:
    const std::unordered_map<dsp::Timing::NoteTiming, std::string> _timingToNames = dsp::Timing::getNoteTimingToNames();
};

}
