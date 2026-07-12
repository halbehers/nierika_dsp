#pragma once

#include <string>

#include "MultiWaySwitch.h"

namespace nierika::gui::element
{

class ThreeWaySwitch: public MultiWaySwitch
{
public:
    using OnValueChangedListener = MultiWaySwitch::OnValueChangedListener;

    ThreeWaySwitch(const std::string& identifier, const std::string& label0, const std::string& label1, const std::string& label2);
    ~ThreeWaySwitch() override = default;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThreeWaySwitch)
};

}
