#pragma once

#include "../laf/BoxToggle.h"

namespace nierika::gui::element
{

class BoxToggle: public juce::ToggleButton
{
public:
    BoxToggle();
    ~BoxToggle() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void setActive(bool isActive);
    bool isActive();
    
private:
    laf::BoxToggle _lookAndFeel;
    bool _isActive = false;
};

}
