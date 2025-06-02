#include "../../../include/gui/element/BPMSelector.h"

namespace nierika::gui::element
{

BPMSelector::BPMSelector(const std::string& identifier, float minValue, float maxValue, float defaultValue):
    Component(identifier),
    _minValue(minValue),
    _maxValue(maxValue),
    _defaultValue(defaultValue),
    _gridLayout(*this)
{
    setup();
}

BPMSelector::BPMSelector(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Component(parameterID, parameterManager.getParameterDisplayName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _minValue(parameterManager.getParameterMinValue<float>(parameterID, 0.f)),
    _maxValue(parameterManager.getParameterMaxValue<float>(parameterID, 0.f)),
    _defaultValue(parameterManager.getParameterDefaultValue<float>(parameterID, 0.f)),
    _gridLayout(*this)
{
    setup();
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameterManager.getState(), parameterID, _slider);
}

void BPMSelector::setup()
{
    _slider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    _slider.setRange(_minValue, _maxValue);
    _slider.setValue(_defaultValue);
    _slider.setComponentID(getComponentID());
    _slider.setLookAndFeel(&_lookAndFeel);
    _slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    _minusButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    _plusButton.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    _minusButton.onClick = [this] { _slider.setValue(_slider.getValue() - 0.1); };
    _plusButton.onClick = [this] { _slider.setValue(_slider.getValue() + 0.1); };

    _gridLayout.init({ 1, 1 }, { 2, 3, 3, 2 });
    _gridLayout.addComponent("minus", _minusButton, 0, 0, 1, 2);
    _gridLayout.addComponent("slider", _slider, 0, 1, 2, 2);
    _gridLayout.addComponent("plus", _plusButton, 0, 3, 1, 2);

    _gridLayout.setDisplayGrid(false);
}

void BPMSelector::paint(juce::Graphics& g)
{
    Component::paint(g);
    _gridLayout.paint(g);
}

void BPMSelector::resized()
{
    Component::resized();
    _gridLayout.resized();
}

void BPMSelector::setEnabled(bool isEnabled)
{
    _minusButton.setEnabled(isEnabled);
    _plusButton.setEnabled(isEnabled);
    _slider.setEnabled(isEnabled);
}

}
