#pragma once

#include "../element/SVGButton.h"

namespace nierika::ui::laf
{

class IncrementalButtons : public juce::LookAndFeel_V4
{
public:
    IncrementalButtons();
    ~IncrementalButtons() override;
    
    juce::Button* createSliderButton(juce::Slider& slider, bool isIncrement) override;
};

}
