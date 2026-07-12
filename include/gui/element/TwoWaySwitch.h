#pragma once

#include <string>

#include "MultiWaySwitch.h"

namespace nierika::gui::element
{

class TwoWaySwitch: public MultiWaySwitch
{
public:
    using OnValueChangedListener = MultiWaySwitch::OnValueChangedListener;

    TwoWaySwitch(const std::string& identifier, const std::string& label0, const std::string& label1);
    ~TwoWaySwitch() override = default;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwoWaySwitch)
};

}
