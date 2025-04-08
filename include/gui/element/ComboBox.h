#pragma once

#include "../laf/ComboBox.h"

namespace nierika::gui::element
{

class ComboBox: public juce::ComboBox
{
public:
    ComboBox(const juce::String& name);
    ~ComboBox() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    laf::ComboBox _lookAndFeel;
};

}
