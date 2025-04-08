#pragma once

#include "../laf/IncrementalButtons.h"
#include "Slider.h"

namespace nierika::gui::element
{

class IncrementalButtons: public Slider
{
    public:
    IncrementalButtons(float minValue, float maxValue, float defaultValue);
    ~IncrementalButtons() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    private:
    laf::IncrementalButtons _lookAndFeel;
};

}
