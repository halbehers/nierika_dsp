#pragma once

#include <string>
#include <vector>

#include "../Component.h"

namespace nierika::gui::element
{

// Shared logic for segmented switches with a sliding selection thumb (TwoWaySwitch,
// ThreeWaySwitch, ...) - draws N equal segments, animates the thumb between them, and
// handles click-to-select. Subclasses just provide a fixed-arity, friendlier constructor.
class MultiWaySwitch: public Component, public juce::Timer
{
public:
    struct OnValueChangedListener
    {
        virtual ~OnValueChangedListener() = default;
        virtual void onSelectionChanged(const std::string& componentID, int selectedIndex) = 0;
    };

    MultiWaySwitch(const std::string& identifier, std::vector<std::string> labels);
    ~MultiWaySwitch() override = default;

    void paint(juce::Graphics& g) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void timerCallback() override;

    void setSelectedIndex(int index, juce::NotificationType notification = juce::sendNotification);
    [[nodiscard]] int getSelectedIndex() const { return _selectedIndex; }

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeListener(OnValueChangedListener* listener);

    template<typename... Labels>
    void setLabels(Labels&&... newLabelsInOrder)
    {
        if (sizeof...(newLabelsInOrder) != _labels.size())
        {
            jassertfalse;
            return;
        }

        _labels = { std::string(std::forward<Labels>(newLabelsInOrder))... };
        repaint();
    }

    void setSelectedInvertedTextColor(bool hasInvertedTextColorOnSelected) { _hasInvertedTextColorOnSelected = hasInvertedTextColorOnSelected; }
    bool hasInvertedTextColorOnSelected() const { return _hasInvertedTextColorOnSelected; }

private:
    std::vector<std::string> _labels;
    int _selectedIndex = 0;
    float _thumbPosition = 0.f;
    std::vector<OnValueChangedListener*> _listeners;
    bool _hasInvertedTextColorOnSelected = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiWaySwitch)
};

}
