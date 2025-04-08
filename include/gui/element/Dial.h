#pragma once

#include "Slider.h"
#include "../laf/Dial.h"

namespace nierika::ui::element
{

class Dial: public Slider
{
public:
    enum Size
    {
        SMALL,
        MEDIUM,
        LARGE
    };
    
    Dial(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix);
    Dial(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix, Size size);
    ~Dial() override;
    
    void init();
    void paint (juce::Graphics&) override;
    void resized() override;
    void setSize(Size size);
    void setShortLabel(const juce::String& shortLabel);
    
private:
    ui::laf::Dial _lookAndFeel;
    Size _size;
    juce::String _label;
    std::unordered_map<Size, float> _sizeToPx {
        {
            { SMALL, 32.0 },
            { MEDIUM, 42.0 },
            { LARGE, 55.0 }
        }
    };
};

}
