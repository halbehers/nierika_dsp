#include "../../../include/gui/element/PercentageDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
PercentageDial::PercentageDial(const std::string& identifier, const juce::String& label, float minValue, float maxValue, float defaultValue, Size size):
    Dial(identifier, label, minValue, maxValue, defaultValue, "", size)
{
    _slider.textFromValueFunction = [this](double value) { return getTextFromValue(value); };
}

PercentageDial::~PercentageDial()
{
}

juce::String PercentageDial::getTextFromValue(double value)
{
    return Formatter::formatPercentage((int) (value * 100));
}

}
