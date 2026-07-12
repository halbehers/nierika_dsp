#pragma once

#include <string>
#include <vector>

#include "../Component.h"

namespace nierika::gui::element
{

class ToggleSwitch: public Component, public juce::Timer
{
public:
    struct OnValueChangedListener
    {
        virtual ~OnValueChangedListener() = default;
        virtual void onToggleValueChanged(const std::string& componentID, bool isOn) = 0;
    };

    explicit ToggleSwitch(const std::string& identifier);
    ~ToggleSwitch() override = default;

    void paint(juce::Graphics& g) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void timerCallback() override;

    void setToggleState(bool isOn, juce::NotificationType notification = juce::sendNotification);
    [[nodiscard]] bool getToggleState() const { return _isOn; }

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeListener(OnValueChangedListener* listener);


    void setSelectedInvertedColor(bool hasInvertedColorOnSelected) { _hasInvertedColorOnSelected = hasInvertedColorOnSelected; }
    bool hasInvertedColorOnSelected() const { return _hasInvertedColorOnSelected; }

private:
    bool _isOn = false;
    float _thumbPosition = 0.f;
    std::vector<OnValueChangedListener*> _listeners;
    bool _hasInvertedColorOnSelected = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToggleSwitch)
};

}
