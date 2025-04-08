#include "../../../includes/gui/element/IncrementalButtons.h"

namespace ui::element
{

IncrementalButtons::IncrementalButtons(float minValue, float maxValue, float defaultValue):
    Slider("", minValue, maxValue, defaultValue, "")
{
    setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    setLookAndFeel(&_lookAndFeel);
}

IncrementalButtons::~IncrementalButtons()
{
}

void IncrementalButtons::paint(juce::Graphics& g)
{
    Slider::paint(g);
}

void IncrementalButtons::resized()
{
    Slider::resized();
    setBounds(getX(), getY(), 40, 16);
}

}
