#pragma once

#include "../Theme.h"

namespace nierika::gui::laf
{

class BoxToggle : public juce::LookAndFeel_V4
{
public:
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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoxToggle)
};

}
