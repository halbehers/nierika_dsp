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

    void setHeightType(Theme::HeightType type) { _heightType = type; repaint(); }
    [[nodiscard]] Theme::HeightType getHeightType() const { return _heightType; }

    void setBackgroundColour(juce::Colour colour) { _backgroundOverride = colour; }
    void resetBackgroundColour() { _backgroundOverride = juce::Colour(); }
    juce::Colour getBackgroundColour() const { return _backgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce()); }

    void setBorderColour(juce::Colour colour) { _borderOverride = colour; }
    void resetBorderColour() { _borderOverride = juce::Colour(); }
    juce::Colour getBorderColour() const { return _borderOverride.value_or(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.2f)); }

    void setBorderRadius(float radius) { _borderRadiusOverride = radius; }
    void resetBorderRadius() { _borderRadiusOverride = -1; }
    float getBorderRadius() const { return _borderRadiusOverride >= 0 ? _borderRadiusOverride : Theme::getBorderRadius(); }

    int getHeight() { return static_cast<int>(Theme::resolveHeight(_heightType, static_cast<float>(getLocalBounds().getHeight()))); }

    void setRounded(bool isRounded) { setBorderRadius(getHeight() / 2); }

private:
    std::vector<std::string> _labels;
    int _selectedIndex = 0;
    float _thumbPosition = 0.f;
    std::vector<OnValueChangedListener*> _listeners;
    bool _hasInvertedTextColorOnSelected = false;
    Theme::HeightType _heightType = Theme::HeightType::AUTO;
    float _borderRadiusOverride = -1;

    std::optional<juce::Colour> _backgroundOverride = std::nullopt;
    std::optional<juce::Colour> _borderOverride = std::nullopt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiWaySwitch)
};

}
