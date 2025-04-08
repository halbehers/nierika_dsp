#pragma once

#include "../laf/IncrementalButtons.h"
#include "Slider.h"

namespace nierika::ui::element
{

class IncrementalButtons: public Slider
{
    public:
    IncrementalButtons(float minValue, float maxValue, float defaultValue);
    ~IncrementalButtons() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    private:
    ui::laf::IncrementalButtons _lookAndFeel;
};

}
