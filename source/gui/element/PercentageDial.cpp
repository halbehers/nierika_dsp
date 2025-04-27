#include "../../../include/gui/element/PercentageDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
PercentageDial::PercentageDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, Size size):
    Dial(identifier, label, minValue, maxValue, defaultValue, "", size)
{
    setup();
}

PercentageDial::PercentageDial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, Size size):
    Dial(parameterManager, parameterID, "", size)
{
    setup();
}

PercentageDial::~PercentageDial()
{
}

void PercentageDial::setup()
{
    _slider.textFromValueFunction = [this](double value) { return getTextFromValue(value); };
}

juce::String PercentageDial::getTextFromValue(double value)
{
    return Formatter::formatPercentage((int) (value * 100));
}

}
