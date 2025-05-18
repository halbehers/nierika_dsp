#pragma once

#include "../Component.h"
#include "../laf/SVGToggle.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

class SVGToggle: public Component
{
public:    
    struct OnValueChangedListener
    {
        virtual ~OnValueChangedListener() = default;
        virtual void onToggleValueChanged(const std::string& componentID, bool isOn) = 0;
    };

    SVGToggle(const std::string& identifier, const char* svgBinary);
    SVGToggle(dsp::ParameterManager& parameterManager, const std::string& parameterID, const char* svgBinary);
    ~SVGToggle() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeListener(OnValueChangedListener* listener);

    bool getToggleState() const;
    void setHelpText(const std::string& helpText);

private:
    laf::SVGToggle _lookAndFeel;
    juce::ToggleButton _button {};

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _attachment {};

    std::vector<OnValueChangedListener*> _listeners;

    void setup();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGToggle)
};

}
