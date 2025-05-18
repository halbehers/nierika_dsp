#pragma once

#include "../laf/SVGButton.h"

namespace nierika::gui::element
{

class SVGButton: public juce::DrawableButton
{
public:
    explicit SVGButton(const char* svgBinary);
    SVGButton(const char* svgBinary, int width, int height);
    ~SVGButton() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    laf::SVGButton _lookAndFeel;
    int _width = -1;
    int _height = -1;

    void init();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGButton)
};

}
