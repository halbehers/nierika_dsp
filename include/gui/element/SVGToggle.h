#pragma once

#include "../Component.h"
#include "../Helpers.h"
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

    void paint(juce::Graphics& _g) override;
    void resized() override;

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeListener(OnValueChangedListener* listener);

    bool getToggleState() const;
    void setHelpText(const std::string& helpText);

    const char* getIconBinary() const { return _svgBinary; }
    void setIconBinary(const char* svgBinary) { _svgBinary = svgBinary; _button.lookAndFeelChanged(); }

    void setIconSize(float size) { _iconSize = size; resized(); }
    void resetIconSize() { _iconSize = -1.f; resized(); }
    float getIconSize() const { return _iconSize >= 0.f ? _iconSize : static_cast<float>(juce::jmin(getWidth(), getHeight())); }

    void setClickableSurface(const helpers::ClickableSurface clickableSurface) { _clickableSurface = clickableSurface; resized(); }
    helpers::ClickableSurface getClickableSurface() const { return _clickableSurface; }

    void setEnabled(bool shouldBeEnabled);

private:
    laf::SVGToggle _lookAndFeel { *this };
    juce::ToggleButton _button {};

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _attachment {};

    const char* _svgBinary;
    float _iconSize = -1.f;
    helpers::ClickableSurface _clickableSurface = helpers::ClickableSurface::ALL_AVAILABLE_AREA;

    std::vector<OnValueChangedListener*> _listeners;

    void setup();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGToggle)
};

}
