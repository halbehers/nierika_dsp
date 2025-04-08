#include "../../../include/gui/element/PercentageDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
PercentageDial::PercentageDial(const juce::String& label, float minValue, float maxValue, float defaultValue, Size size):
    Dial(label, minValue, maxValue, defaultValue, "", size)
{
}

PercentageDial::~PercentageDial()
{
}

juce::String PercentageDial::getTextFromValue(double value)
{
    return Formatter::formatPercentage((int) (value * 100));
}

}
