#pragma once

#include "../laf/IncrementalButtons.h"
#include "Slider.h"

namespace nierika::gui::element
{

class IncrementalButtons: public Slider
{
public:
    IncrementalButtons(float minValue, float maxValue, float defaultValue);
    ~IncrementalButtons() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    laf::IncrementalButtons _lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IncrementalButtons)
};

}
