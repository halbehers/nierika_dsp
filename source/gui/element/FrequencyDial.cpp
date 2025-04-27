#include "../../../include/gui/element/FrequencyDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
FrequencyDial::FrequencyDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, Size size):
    Dial(identifier, label, minValue, maxValue, defaultValue, "", size)
{
    setup();
}

FrequencyDial::FrequencyDial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, Size size):
    Dial(parameterManager, parameterID, "", size)
{
    setup();
}

FrequencyDial::~FrequencyDial()
{
}

void FrequencyDial::setup()
{
    _slider.textFromValueFunction = [this](double value) { return getTextFromValue(value); };
}

juce::String FrequencyDial::getTextFromValue(double value)
{
    return Formatter::formatFrequency((float) value);
}

}
