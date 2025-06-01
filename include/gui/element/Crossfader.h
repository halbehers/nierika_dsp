#pragma once

#include "../Component.h"
#include "../../dsp/ParameterManager.h"
#include "nierika_dsp/include/gui/laf/HorizontalSlider.h"

namespace nierika::gui::element
{

class Crossfader final: public Component
{
public:
    explicit Crossfader(const std::string& identifier, float defaultValue = 0.f);
    Crossfader(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~Crossfader() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void setEnabled(bool isEnabled);

private:
    float _defaultValue;

    juce::Slider _slider {};
    laf::HorizontalSlider _lookAndFeel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _attachment {};
    float _aValueOpacity = 0.f;
    float _bValueOpacity = 0.f;

    void setup();

    static float computeOpacityFromValue(float value);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Crossfader)
};

}
