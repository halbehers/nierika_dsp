#pragma once

#include "../laf/SVGButton.h"

namespace nierika::gui::element
{

class IncrementalButton final: public juce::ArrowButton
{
public:
    enum Type
    {
        PLUS,
        MINUS
    };

    IncrementalButton(const juce::String& buttonName, Type type);
    explicit IncrementalButton(Type type);
    ~IncrementalButton() override;

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    Type _type;
    const char* _svgBinary;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IncrementalButton)
};

}
