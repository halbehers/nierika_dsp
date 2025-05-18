#pragma once

namespace nierika::gui::element
{

class Slider: public juce::Slider
{
public:
    Slider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix);
    ~Slider() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setEnabled(bool isEnabled);

protected:
    juce::Label _labelComponent {};

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Slider)
};

}
