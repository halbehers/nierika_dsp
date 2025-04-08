#pragma once

#include "../laf/SVGButton.h"

namespace nierika::ui::element
{

class IncrementalButton: public juce::ArrowButton
{
public:
    enum Type
    {
        PLUS,
        MINUS
    };
    
    IncrementalButton(const juce::String& buttonName, Type type);
    IncrementalButton(Type type);
    ~IncrementalButton() override;
    
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    Type _type;
    const char* _svgBinary;
};

}
