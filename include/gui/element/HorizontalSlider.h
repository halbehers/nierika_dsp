#pragma once

#include "Slider.h"

namespace nierika::gui::element
{

class HorizontalSlider: public Slider
{
    public:
    HorizontalSlider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix);
    ~HorizontalSlider() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
};

}
