#include "../../../include/gui/element/TimeInMsDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
TimeInMsDial::TimeInMsDial(const juce::String& label, float minValue, float maxValue, float defaultValue, Size size):
    Dial(label, minValue, maxValue, defaultValue, "", size)
{
}

TimeInMsDial::~TimeInMsDial()
{
}

juce::String TimeInMsDial::getTextFromValue(double value)
{
    return Formatter::formatTimeInMs(value);
}

}
