#pragma once

#include "../Theme.h"

namespace nierika::ui::laf
{

struct BoxToggle : public juce::LookAndFeel_V4
{
    BoxToggle();
    ~BoxToggle() override;

    void drawTickBox
    (
        juce::Graphics& g,
        juce::Component& button,
        float x,
        float y,
        float w,
        float h,
        bool ticked,
        bool isEnabled,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown
     ) override;
};

}
