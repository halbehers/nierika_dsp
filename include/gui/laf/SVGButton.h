#pragma once

#include "../Theme.h"

namespace nierika::ui::laf
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

};

}
