#include "../../../include/gui/element/Slider.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Slider::Slider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix)
{
    setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 10);
    setRange(minValue, maxValue);
    setValue(defaultValue);
    auto transparentColor = Theme::getInstance().getColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    auto accentColor = Theme::getInstance().getColor(Theme::ThemeColor::ACCENT).asJuce();
    setColour(juce::Slider::ColourIds::rotarySliderFillColourId, accentColor);
    setColour(juce::Slider::ColourIds::trackColourId, accentColor);
    setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, transparentColor);
    setColour(juce::Slider::ColourIds::thumbColourId, whiteColor);
    setColour(juce::Slider::ColourIds::textBoxOutlineColourId, transparentColor);

    if (!valueSuffix.isEmpty()) {
        setTextValueSuffix(" " + valueSuffix);
    }
    setDoubleClickReturnValue(true, defaultValue);
    
    /**addAndMakeVisible(_labelComponent);
     _labelComponent.setText(label, juce::dontSendNotification);
     _labelComponent.setFont(EmbeddedFonts::getRegular().withHeight(16.0).withStyle(juce::Font::italic));
     _labelComponent.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour(COLOR_TRANSPARENT));
     _labelComponent.setColour(juce::Label::ColourIds::backgroundWhenEditingColourId, juce::Colour(COLOR_TRANSPARENT));
     _labelComponent.setColour(juce::Label::ColourIds::textColourId, isEnabled() ? juce::Colour(COLOR_TEXT) : juce::Colour(COLOR_DISABLED));
     _labelComponent.attachToComponent(this, true);
     _labelComponent.setJustificationType(juce::Justification::centred);
     */
}

void Slider::setEnabled(bool isEnabled)
{
    juce::Slider::setEnabled(isEnabled);
    auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    auto disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED).asJuce();
    _labelComponent.setColour(juce::Label::ColourIds::textColourId, isEnabled ? whiteColor : disabledColor);
}

void Slider::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);
}

void Slider::resized()
{
    juce::Slider::resized();
}

}
