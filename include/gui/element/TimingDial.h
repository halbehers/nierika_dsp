#pragma once

#include "Dial.h"
#include "../../dsp/Timing.h"

namespace nierika::ui::element
{

class TimingDial: public Dial
{
public:
    TimingDial(const juce::String& label, dsp::Timing::NoteTiming defaultValue, Size size = MEDIUM);
    ~TimingDial() override;

    juce::String getTextFromValue(double value) override;

private:
    const std::unordered_map<dsp::Timing::NoteTiming, std::string> _timingToNames = dsp::Timing::getNoteTimingToNames();
};

}
