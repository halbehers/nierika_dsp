#pragma once

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

    void setPadding(int padding);
    int getPadding() const;

private:
    Type _type;
    const char* _svgBinary;
    int _padding = 16;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IncrementalButton)
};

}
