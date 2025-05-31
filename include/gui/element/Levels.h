#pragma once

#include "../Component.h"
#include "../../dsp/RMSProcessor.h"
#include "../layout/GridLayout.h"

namespace nierika::gui::element
{

class Levels : public Component, public juce::Timer
{
public:
    explicit Levels(const dsp::RMSProcessor& rmsProcessor);
    Levels(const std::string& identifier, const dsp::RMSProcessor& rmsProcessor);
    ~Levels() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;
    
private:
    const dsp::RMSProcessor& _rmsProcessor;
    Level _levelMeterLeft;
    Level _levelMeterRight;

    layout::GridLayout<juce::Component> _layout;

    void init();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Levels)
};

}
