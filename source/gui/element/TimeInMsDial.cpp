#include "../../../include/gui/element/TimeInMsDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
TimeInMsDial::TimeInMsDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue):
    Dial(identifier, label, minValue, maxValue, defaultValue, "")
{
    TimeInMsDial::setup();
}

TimeInMsDial::TimeInMsDial(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Dial(parameterManager, parameterID, "")
{
    TimeInMsDial::setup();
}

void TimeInMsDial::setup()
{
    _slider.textFromValueFunction = [this](double value) { return getTextFromValue(value); };
}

juce::String TimeInMsDial::getTextFromValue(double value)
{
    return Formatter::formatTimeInMs(value);
}

}
