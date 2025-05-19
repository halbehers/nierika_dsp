#pragma once

namespace nierika::gui::element
{

class ComboBox: public juce::ComboBox
{
public:
    explicit ComboBox(const juce::String& name);
    ~ComboBox() override = default;
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    laf::ComboBox _lookAndFeel;
};

}
