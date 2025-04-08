#pragma once

#include "ComboBox.h"

namespace nierika::gui::element
{

class TimingComboBox: public ComboBox
{
public:
    TimingComboBox(const juce::String& name);
    ~TimingComboBox() override;
};

}
