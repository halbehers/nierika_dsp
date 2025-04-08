#include "../../../include/gui/element/Dial.h"
#include "../../../include/gui/Theme.h"

namespace ui::element
{

Dial::Dial(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix):
    Slider(label, minValue, maxValue, defaultValue, valueSuffix),
    _size(MEDIUM),
    _label(label)
{
    init();
    
}

Dial::Dial(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix, Size size):
    Slider(label, minValue, maxValue, defaultValue, valueSuffix),
    _size(size),
    _label(label)
{
    init();
}

Dial::~Dial()
{
}

void Dial::init()
{
    setName(_label);
    setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
    setLookAndFeel(&_lookAndFeel);
}

void Dial::setSize(Size size)
{
    _size = size;
}

void Dial::setShortLabel(const juce::String& shortLabel)
{
    _lookAndFeel.setShortLabel(shortLabel);
}

void Dial::paint (juce::Graphics& g)
{
    Slider::paint(g);
}

void Dial::resized()
{
    Slider::resized();
    float width = _sizeToPx[_size];
    float height = width + 26;
    setBounds(getX(), getY(), (int) height, (int) width);
}

}
