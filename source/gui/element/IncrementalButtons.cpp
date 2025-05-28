#include "../../../include/gui/element/IncrementalButtons.h"

namespace nierika::gui::element
{

IncrementalButtons::IncrementalButtons(const std::string& identifier, float minValue, float maxValue, float defaultValue):
    Component(identifier),
    _minValue(minValue),
    _maxValue(maxValue),
    _defaultValue(defaultValue)
{
    setup();
}

IncrementalButtons::IncrementalButtons(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Component(parameterID, parameterManager.getParameterDisplayName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _minValue(parameterManager.getParameterMinValue<float>(parameterID, 0.f)),
    _maxValue(parameterManager.getParameterMaxValue<float>(parameterID, 0.f)),
    _defaultValue(parameterManager.getParameterDefaultValue<float>(parameterID, 0.f))
{
    setup();
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameterManager.getState(), parameterID, _slider);
}

void IncrementalButtons::setup()
{
    addAndMakeVisible(_slider);

    _slider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);

    _slider.setRange(_minValue, _maxValue);
    _slider.setValue(_defaultValue);
    _slider.setLookAndFeel(&_lookAndFeel);
    _slider.setComponentID(getComponentID());
}

void IncrementalButtons::resized()
{
    Component::resized();
    setBounds(getX(), getY(), 40, 16);
}


void IncrementalButtons::setEnabled(bool isEnabled)
{
    _slider.setEnabled(isEnabled);
}

}
