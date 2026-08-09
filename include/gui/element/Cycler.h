#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../Component.h"
#include "../../dsp/ParameterManager.h"
#include "./SVGButton.h"
#include "./Text.h"

namespace nierika::gui::element
{

// A generic "< value >" cycling control - left/right SVGButton arrows either side of a centered
// label, stepping through a fixed ordered list of string choices. Follows ComboBox's construction
// pattern: two constructors (manual choices, or APVTS-attached to a juce::AudioParameterChoice),
// theme colors always via Theme::newColor(...), JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR.
class Cycler: public Component, public SVGButton::OnClickListener
{
public:
    struct OnValueChangedListener
    {
        virtual ~OnValueChangedListener() = default;
        virtual void onSelectionChanged(const std::string& componentID, int selectedIndex) = 0;
    };

    Cycler(const std::string& identifier, const std::vector<std::string>& choices, int defaultIndex = 0);

    // parameterID must name a juce::AudioParameterChoice registered on parameterManager - its own
    // choices list is read directly from the parameter, so it never needs to be passed separately.
    Cycler(dsp::ParameterManager& parameterManager, const std::string& parameterID);

    ~Cycler() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setSelectedIndex(int index);
    [[nodiscard]] int getSelectedIndex() const { return _selectedIndex; }
    [[nodiscard]] std::string getSelectedText() const;

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeListener(OnValueChangedListener* listener);

    void onButtonClick(const std::string& componentID) override;

    void setBackgroundColour(juce::Colour colour) { _backgroundOverride = colour; refreshBackgroundAndBorder(); }
    void resetBackgroundColour() { _backgroundOverride = juce::Colour(); }
    juce::Colour getBackgroundColour() const { return _backgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce()); }

    void setBorderColour(juce::Colour colour) { _borderOverride = colour; refreshBackgroundAndBorder(); }
    void resetBorderColour() { _borderOverride = juce::Colour(); }
    juce::Colour getBorderColour() const { return _borderOverride.value_or(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.2f)); }

    void setBorderRadius(float radius) { _borderRadiusOverride = radius; refreshBackgroundAndBorder(); }
    void resetBorderRadius() { _borderRadiusOverride = -1; }
    float getBorderRadius() const { return _borderRadiusOverride >= 0 ? _borderRadiusOverride : Theme::getBorderRadius(); }

    void setRounded(bool isRounded) { if (isRounded) setBorderRadius(static_cast<float>(getHeight()) / 2.f); else resetBorderRadius(); }

    void refreshBackgroundAndBorder() { displayBackground(getBackgroundColour(), getBorderRadius()); displayBorder(getBorderColour(), 1.f, getBorderRadius()); }


private:
    void setup();
    void refreshLabel();
    void applyIndexFromUI(int newIndex);
    std::optional<juce::Colour> _backgroundOverride = std::nullopt;
    std::optional<juce::Colour> _borderOverride = std::nullopt;
    float _borderRadiusOverride = -1;

    std::vector<std::string> _choices;
    int _selectedIndex = 0;

    SVGButton _leftArrow;
    SVGButton _rightArrow;
    Text _label;

    std::unique_ptr<juce::ParameterAttachment> _attachment;

    std::vector<OnValueChangedListener*> _listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Cycler)
};

}
