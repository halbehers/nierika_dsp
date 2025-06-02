#include "../../../include/gui/element/Dial.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Dial::Dial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, const std::string& valueSuffix):
    Component(identifier, label),
    _minValue(minValue),
    _maxValue(maxValue),
    _defaultValue(defaultValue),
    _valueSuffix(valueSuffix)
{
    Dial::setup();
}

Dial::Dial(dsp::ParameterManager& parameterManager, const std::string& parameterID, const std::string& valueSuffix):
    Component(parameterID, parameterManager.getParameterDisplayName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _minValue(parameterManager.getParameterMinValue<float>(parameterID, 0.f)),
    _maxValue(parameterManager.getParameterMaxValue<float>(parameterID, 0.f)),
    _defaultValue(parameterManager.getParameterDefaultValue<float>(parameterID, 0.f)),
    _valueSuffix(valueSuffix)
{
    Dial::setup();
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameterManager.getState(), parameterID, _slider);
}

Dial::~Dial()
{
}

void Dial::setup()
{
    addAndMakeVisible(_slider);
    
    _slider.setName(getName());
    _slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    _slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 17);
    _slider.setRange(_minValue, _maxValue);
    _slider.setValue(_defaultValue);
    _slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    const juce::Colour transparentColor = Theme::newColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    const juce::Colour whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    const juce::Colour accentColor = Theme::newColor(Theme::ThemeColor::ACCENT).asJuce();
    _slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, accentColor);
    _slider.setColour(juce::Slider::ColourIds::trackColourId, accentColor);
    _slider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, transparentColor);
    _slider.setColour(juce::Slider::ColourIds::thumbColourId, whiteColor);
    _slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, transparentColor);

    if (_valueSuffix != "") {
        _slider.setTextValueSuffix(" " + _valueSuffix);
    }
    _slider.setDoubleClickReturnValue(true, _defaultValue);
    _slider.setLookAndFeel(&_lookAndFeel);
    _slider.setComponentID(getComponentID());
}

void Dial::setShortLabel(const juce::String& shortLabel)
{
    _lookAndFeel.setShortLabel(shortLabel);
}

void Dial::resized()
{
    Component::resized();
    _slider.setBounds(getLocalBounds());
}

void Dial::setEnabled(bool isEnabled)
{
    _slider.setEnabled(isEnabled);
}

void Dial::setLabelVisibility(laf::Dial::LabelVisibility visibility)
{
    _lookAndFeel.setLabelVisibility(visibility);
}

}
