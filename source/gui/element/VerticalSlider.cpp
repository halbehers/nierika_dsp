#include "../../../include/gui/element/VerticalSlider.h"

namespace nierika::gui::element
{

VerticalSlider::VerticalSlider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix):
    Slider(label, minValue, maxValue, defaultValue, valueSuffix)
{
    setup();
}

VerticalSlider::VerticalSlider(dsp::ParameterManager& parameterManager, const std::string& parameterID, const std::string& valueSuffix):
    Slider(parameterManager.getParameterDisplayName(parameterID),
           parameterManager.getParameterMinValue<float>(parameterID, 0.f),
           parameterManager.getParameterMaxValue<float>(parameterID, 0.f),
           parameterManager.getParameterDefaultValue<float>(parameterID, 0.f),
           valueSuffix)
{
    setup();
    setTooltip(parameterManager.getParameterTooltip(parameterID));
    _attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameterManager.getState(), parameterID, *this);
}

VerticalSlider::~VerticalSlider()
{
    Theme::getChangeBroadcaster().removeChangeListener(this);
}

void VerticalSlider::setup()
{
    setLookAndFeel(&_lookAndFeel);
    Theme::getChangeBroadcaster().addChangeListener(this);
}

void VerticalSlider::changeListenerCallback(juce::ChangeBroadcaster*)
{
    if (!_trackColourIsRawOverride)
        setColour(juce::Slider::ColourIds::trackColourId, Theme::newColor(_trackColorIdOverride.value_or(Theme::ThemeColor::ACCENT)).asJuce());
    if (!_thumbColourIsRawOverride)
        setColour(juce::Slider::ColourIds::thumbColourId, Theme::newColor(_thumbColorIdOverride.value_or(Theme::ThemeColor::EMPTY_SHADE)).asJuce());

    // The value/label text box is a separate Label the LookAndFeel creates once
    // (laf::VerticalSlider::createSliderTextBox) and caches its own colours/font into -
    // lookAndFeelChanged() forces it to be recreated (and thus re-themed), same mechanism Dial
    // uses for its own text box.
    lookAndFeelChanged();
}

void VerticalSlider::setTrackColour(juce::Colour colour)
{
    _trackColourIsRawOverride = true;
    setColour(juce::Slider::ColourIds::trackColourId, colour);
}

void VerticalSlider::setTrackColour(Theme::ThemeColor colorId)
{
    _trackColourIsRawOverride = false;
    _trackColorIdOverride = colorId;
    setColour(juce::Slider::ColourIds::trackColourId, Theme::newColor(colorId).asJuce());
}

void VerticalSlider::setThumbColour(juce::Colour colour)
{
    _thumbColourIsRawOverride = true;
    setColour(juce::Slider::ColourIds::thumbColourId, colour);
}

void VerticalSlider::setThumbColour(Theme::ThemeColor colorId)
{
    _thumbColourIsRawOverride = false;
    _thumbColorIdOverride = colorId;
    setColour(juce::Slider::ColourIds::thumbColourId, Theme::newColor(colorId).asJuce());
}

}
