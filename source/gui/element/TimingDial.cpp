#include "../../../include/gui/element/TimingDial.h"
#include "../../../include/dsp/Timing.h"

namespace nierika::gui::element
{
TimingDial::TimingDial(const std::string& identifier, const std::string& label, dsp::Timing::NoteTiming defaultValue, Size size):
    Dial(identifier, label, dsp::Timing::NoteTiming::NOTE_32, dsp::Timing::NoteTiming::NOTE_FOUR_STEP, defaultValue, "", size)
{
    setup();
}

TimingDial::TimingDial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, Size size):
    Dial(parameterManager, parameterID, "", size)
{
    setup();
}

TimingDial::~TimingDial()
{
}

void TimingDial::setup()
{
    _slider.textFromValueFunction = [this](double value) { return getTextFromValue(value); };
}

juce::String TimingDial::getTextFromValue(double value)
{
    dsp::Timing::NoteTiming timing = dsp::Timing::NoteTiming(static_cast<int>(value));

    return _timingToNames.at(timing);
}

}
