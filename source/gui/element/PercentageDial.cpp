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
    // value is already the parameter's raw denormalized value, and every PercentageDial in
    // practice is registered on a 0-100 raw range (matching this codebase's "_PERCENT" convention),
    // not 0-1 - multiplying by 100 here would inflate any nonzero value past Formatter's [0,100]
    // clamp, so the readout previously showed only ever "0%" or "100%", never anything between.
    return Formatter::formatPercentage(static_cast<float>(value));
}

}
