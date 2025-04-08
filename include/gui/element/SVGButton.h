#pragma once

#include "../laf/SVGButton.h"

namespace nierika::gui::element
{

class SVGButton: public juce::DrawableButton
{
public:
    SVGButton(const char* svgBinary);
    SVGButton(const char* svgBinary, int width, int height);
    ~SVGButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    laf::SVGButton _lookAndFeel;
    int _width = -1;
    int _height = -1;

    void init();
};

}
