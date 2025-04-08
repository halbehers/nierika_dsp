#include "../../../include/gui/element/HorizontalSlider.h"

namespace nierika::gui::element
{

HorizontalSlider::HorizontalSlider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix):
Slider(label, minValue, maxValue, defaultValue, valueSuffix)
{
    setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    setTextBoxStyle(juce::Slider::TextBoxRight, true, 80, 10);

    _labelComponent.attachToComponent(this, false);
    _labelComponent.setJustificationType(juce::Justification::left);
}

HorizontalSlider::~HorizontalSlider()
{
}

void HorizontalSlider::paint (juce::Graphics& g)
{
    Slider::paint(g);
}

void HorizontalSlider::resized()
{
    Slider::resized();
    setBounds(getX(), getY() + 8, 150, 16);
}

}
