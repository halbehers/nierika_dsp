#include "../../../include/gui/element/FrequencyDial.h"
#include "../../../include/gui/Formatter.h"

namespace nierika::gui::element
{
FrequencyDial::FrequencyDial(const std::string& identifier, const juce::String& label, float minValue, float maxValue, float defaultValue, Size size):
    Dial(identifier, label, minValue, maxValue, defaultValue, "", size)
{
}

FrequencyDial::~FrequencyDial()
{
}

juce::String FrequencyDial::getTextFromValue(double value)
{
    return Formatter::formatFrequency((float) value);
}

}
