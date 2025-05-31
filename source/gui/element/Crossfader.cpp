#include "../../../include/gui/element/Crossfader.h"

namespace nierika::gui::element
{

Crossfader::Crossfader(const std::string& identifier, float defaultValue):
    Component(identifier),
    _defaultValue(defaultValue)
{
    setup();
}

Crossfader::Crossfader(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Component(parameterID, parameterManager.getParameterDisplayName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _defaultValue(parameterManager.getParameterDefaultValue<float>(parameterID, 0.f))
{
    setup();
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameterManager.getState(), parameterID, _slider);
}

void Crossfader::setup()
{
    addAndMakeVisible(_slider);
    _slider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    _slider.setRange(0.f, 1.f);
    _slider.setValue(_defaultValue);
    _slider.setComponentID(getComponentID());
    _slider.setLookAndFeel(&_lookAndFeel);

    setPadding(42.f, 0.f);
}

void Crossfader::paint(juce::Graphics& g)
{
    Component::paint(g);

    const juce::Colour disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED).asJuce();
    const juce::Colour whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    constexpr int horizontalPadding = 42;
    const juce::String emptyChar = juce::CharPointer_UTF8("\xC3\xB8");

    g.setColour(_slider.isEnabled() ? whiteColor : disabledColor);
    g.setFont(Theme::getInstance().getFont(Theme::THIN, Theme::TITLE));

    g.drawFittedText(emptyChar, getOuterX(), getOuterY(), getOuterHeight(), horizontalPadding, juce::Justification::centred, 1);
    g.drawFittedText("~", getOuterX() + getOuterWidth() - horizontalPadding, getOuterY(), getOuterHeight(), horizontalPadding, juce::Justification::centred, 1);
}

void Crossfader::resized()
{
    Component::resized();
    _slider.setBounds(getLocalBounds());
}

void Crossfader::setEnabled(bool isEnabled)
{
    _slider.setEnabled(isEnabled);
}

}
