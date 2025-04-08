#include "../../../include/gui/element/TimeInMsDial.h"
#include "../../../include/gui/Formatter.h"

namespace ui::element
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
    return ui::Formatter::formatTimeInMs(value);
}

}
