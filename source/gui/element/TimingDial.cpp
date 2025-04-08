#include "../../../include/gui/element/TimingDial.h"
#include "../../../include/dsp/Timing.h"

namespace ui::element
{
TimingDial::TimingDial(const juce::String& label, dsp::Timing::NoteTiming defaultValue, Size size):
    Dial(label, dsp::Timing::NoteTiming::NOTE_32, dsp::Timing::NoteTiming::NOTE_FOUR_STEP, defaultValue, "", size)
{
}

TimingDial::~TimingDial()
{
}

juce::String TimingDial::getTextFromValue(double value)
{
    dsp::Timing::NoteTiming timing = dsp::Timing::NoteTiming(static_cast<int>(value));

    return _timingToNames.at(timing);
}

}
