#pragma once

#include "ComboBox.h"

namespace nierika::gui::element
{

class TimingComboBox: public ComboBox
{
public:
    TimingComboBox(const juce::String& name);
    ~TimingComboBox() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimingComboBox)
};

}
