#include "../../../include/gui/element/Dial.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Dial::Dial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, const std::string& valueSuffix, Size size):
    Component(identifier, label),
    _minValue(minValue),
    _maxValue(maxValue),
    _defaultValue(defaultValue),
    _valueSuffix(valueSuffix),
    _size(size)
{
    setup();
}

Dial::Dial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, const std::string& valueSuffix, Size size):
    Component(parameterID, parameterManager.getParameterName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _minValue(parameterManager.getParameterMinValue<float>(parameterID)),
    _maxValue(parameterManager.getParameterMaxValue<float>(parameterID)),
    _defaultValue(parameterManager.getParameterDefaultValue<float>(parameterID)),
    _valueSuffix(valueSuffix),
    _size(size)
{
    setup();
}

Dial::~Dial()
{
}

void Dial::setup()
{
    addAndMakeVisible(_slider);
    
    _slider.setName(getName());
    _slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    _slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 10);
    _slider.setRange(_minValue, _maxValue);
    _slider.setValue(_defaultValue);
    auto transparentColor = Theme::getInstance().getColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    auto accentColor = Theme::getInstance().getColor(Theme::ThemeColor::ACCENT).asJuce();
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

void Dial::setSize(Size size)
{
    _size = size;
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

}
