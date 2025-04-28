#include "../../../include/gui/element/FrequencyDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
FrequencyDial::FrequencyDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue):
    Dial(identifier, label, minValue, maxValue, defaultValue, "")
{
    setup();
}

FrequencyDial::FrequencyDial(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Dial(parameterManager, parameterID, "")
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
