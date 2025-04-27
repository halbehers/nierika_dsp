#include "../../../include/gui/element/TimeInMsDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
TimeInMsDial::TimeInMsDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, Size size):
    Dial(identifier, label, minValue, maxValue, defaultValue, "", size)
{
    setup();
}

TimeInMsDial::TimeInMsDial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, Size size):
    Dial(parameterManager, parameterID, "", size)
{
    setup();
}

TimeInMsDial::~TimeInMsDial()
{
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
