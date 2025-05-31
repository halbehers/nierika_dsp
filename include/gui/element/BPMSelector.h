#pragma once

#include "../Component.h"
#include "IncrementalButton.h"
#include "Value.h"
#include "../../dsp/ParameterManager.h"
#include "nierika_dsp/include/gui/laf/PitchSlider.h"
#include "nierika_dsp/include/gui/layout/GridLayout.h"

namespace nierika::gui::element
{

class BPMSelector final: public Component
{
public:
    BPMSelector(const std::string& identifier, float minValue, float maxValue, float defaultValue);
    BPMSelector(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~BPMSelector() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void setEnabled(bool isEnabled);

private:
    float _minValue;
    float _maxValue;
    float _defaultValue;

    juce::Slider _slider;
    IncrementalButton _minusButton {  "Minus", IncrementalButton::MINUS };
    IncrementalButton _plusButton {  "Plus", IncrementalButton::PLUS };
    Value _bpmValue { "bpm-selector--value", "", "BPM" };

    layout::GridLayout<Component> _gridLayout;

    laf::PitchSlider _lookAndFeel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _attachment;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BPMSelector)
};

}
