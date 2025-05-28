#pragma once

#include "../laf/IncrementalButtons.h"
#include "../Component.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

class IncrementalButtons: public Component
{
public:
    IncrementalButtons(const std::string& identifier, float minValue, float maxValue, float defaultValue);
    IncrementalButtons(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~IncrementalButtons() override = default;

    void resized() override;

    void setEnabled(bool isEnabled);

private:
    juce::Slider _slider;
    laf::IncrementalButtons _lookAndFeel;

    float _minValue;
    float _maxValue;
    float _defaultValue;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _attachment;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IncrementalButtons)
};

}
