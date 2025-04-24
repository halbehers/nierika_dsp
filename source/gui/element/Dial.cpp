#include "../../../include/gui/element/Dial.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Dial::Dial(const std::string& identifier, const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix, Size size):
    Component(identifier),
    _size(size)
{
    setComponentID(identifier);
    addAndMakeVisible(_slider);
    
    _slider.setName(label);
    _slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    _slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 10);
    _slider.setRange(minValue, maxValue);
    _slider. setValue(defaultValue);
    auto transparentColor = Theme::getInstance().getColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    auto accentColor = Theme::getInstance().getColor(Theme::ThemeColor::ACCENT).asJuce();
    _slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, accentColor);
    _slider.setColour(juce::Slider::ColourIds::trackColourId, accentColor);
    _slider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, transparentColor);
    _slider.setColour(juce::Slider::ColourIds::thumbColourId, whiteColor);
    _slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, transparentColor);

    if (!valueSuffix.isEmpty()) {
        _slider.setTextValueSuffix(" " + valueSuffix);
    }
    _slider.setDoubleClickReturnValue(true, defaultValue);
    _slider.setLookAndFeel(&_lookAndFeel);
    _slider.setComponentID(identifier);
}

Dial::~Dial()
{
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
