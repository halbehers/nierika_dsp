#include "../../../include/gui/element/PercentageDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
PercentageDial::PercentageDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue):
    Dial(identifier, label, minValue, maxValue, defaultValue, "")
{
    PercentageDial::setup();
}

PercentageDial::PercentageDial(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Dial(parameterManager, parameterID, "")
{
    PercentageDial::setup();
}

void PercentageDial::setup()
{
    _slider.textFromValueFunction = [](double value) { return getTextFromValue(value); };
}

juce::String PercentageDial::getTextFromValue(double value)
{
    return Formatter::formatPercentage((int) (value * 100));
}

}
