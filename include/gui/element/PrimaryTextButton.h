#pragma once

#include "TextButton.h"
#include "../laf/PrimaryTextButton.h"

namespace nierika::gui::element
{

class PrimaryTextButton: public TextButton
{
public:    
    PrimaryTextButton(const std::string& identifier, const std::string& text);
    ~PrimaryTextButton() override = default;

private:
    laf::PrimaryTextButton _lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrimaryTextButton)
};

}
 
