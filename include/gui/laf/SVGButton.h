#pragma once

#include "../Theme.h"

namespace nierika::gui::laf
{

class SVGButton : public juce::LookAndFeel_V4
{
public:
    SVGButton();
    SVGButton(const char* svgBinary);
    ~SVGButton() override;

    void drawDrawableButton
    (
        juce::Graphics &g,
        juce::DrawableButton &button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown
    ) override;

private:
    const char* _svgBinary;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGButton)
};

}
