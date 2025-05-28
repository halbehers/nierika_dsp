#include "../../../include/gui/element/DJFilterDial.h"

namespace nierika::gui::element
{
DJFilterDial::DJFilterDial(const std::string& identifier, const std::string& label, float defaultValue):
    Dial(identifier, label, -1.f, 1.f, defaultValue, "")
{
    DJFilterDial::setup();
}

DJFilterDial::DJFilterDial(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Dial(parameterManager, parameterID, "")
{
    DJFilterDial::setup();
}

void DJFilterDial::setup()
{
    _lookAndFeel.setLabelVisibility(laf::Dial::VALUE_ONLY);
    _slider.textFromValueFunction = [](const double value) { return getTextFromValue(value); };
}

juce::String DJFilterDial::getTextFromValue(double value)
{
    if (utils::FloatingPointUtils::isEmpty(value)) return "Filter";

    return utils::FloatingPointUtils::isPositive(value) ? "High Pass Filter" : "Low Pass Filter";
}

}
