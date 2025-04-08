#pragma once

#include "../laf/SVGToggle.h"

namespace nierika::gui::element
{

class SVGToggle: public juce::ToggleButton
{
public:
    SVGToggle(const char* svgBinary);
    SVGToggle(const char* svgBinary, int width, int height);
    ~SVGToggle() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    ui::laf::SVGToggle _lookAndFeel;
    int _width = -1;
    int _height = -1;

    void init();
};

}
