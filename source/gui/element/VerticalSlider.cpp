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

VerticalSlider::~VerticalSlider() = default;

void VerticalSlider::setup()
{
    setLookAndFeel(&_lookAndFeel);
}

void VerticalSlider::setTrackColour(juce::Colour colour)
{
    setColour(juce::Slider::ColourIds::trackColourId, colour);
}

void VerticalSlider::setThumbColour(juce::Colour colour)
{
    setColour(juce::Slider::ColourIds::thumbColourId, colour);
}

}
