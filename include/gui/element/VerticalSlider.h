#pragma once

#include <optional>

#include "./Slider.h"
#include "../laf/VerticalSlider.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

// Extends Slider (already vertical-styled by default) with a filled-pill LookAndFeel (track/thumb
// colour, border radius) and Dial-style extras: font size/weight overrides, and a hover-swap label
// (shows the static name normally, the live formatted value while hovering/dragging - identical
// mechanism to laf::Dial::drawLabel, see laf::VerticalSlider).
class VerticalSlider: public Slider
{
public:
    VerticalSlider(const juce::String& label, float minValue, float maxValue, float defaultValue, const juce::String& valueSuffix = "");

    // parameterID must name a registered float-ranged parameter - mirrors Dial's APVTS-attached
    // constructor pattern (min/max/default/display-name all read from the parameter itself).
    VerticalSlider(dsp::ParameterManager& parameterManager, const std::string& parameterID, const std::string& valueSuffix = "");
    ~VerticalSlider() override;

    // juce::Colour primary, matching Cycler::setBackgroundColour's existing convention; a
    // Theme::ThemeColor overload layered on top for the common app-theme case.
    void setTrackColour(juce::Colour colour);
    void setTrackColour(Theme::ThemeColor colorId) { setTrackColour(Theme::newColor(colorId).asJuce()); }
    void setThumbColour(juce::Colour colour);
    void setThumbColour(Theme::ThemeColor colorId) { setThumbColour(Theme::newColor(colorId).asJuce()); }

    void setBorderRadius(float radius) { _borderRadiusOverride = radius; lookAndFeelChanged(); }
    void resetBorderRadius() { _borderRadiusOverride = std::nullopt; lookAndFeelChanged(); }
    [[nodiscard]] float getBorderRadius() const { return _borderRadiusOverride.value_or(Theme::getBorderRadius()); }

    // The thumb (filled value bar)'s own corner radius, independent of the track's - defaults to
    // the same app-wide Theme::getBorderRadius() unless overridden.
    void setThumbBorderRadius(float radius) { _thumbBorderRadiusOverride = radius; lookAndFeelChanged(); }
    void resetThumbBorderRadius() { _thumbBorderRadiusOverride = std::nullopt; lookAndFeelChanged(); }
    [[nodiscard]] float getThumbBorderRadius() const { return _thumbBorderRadiusOverride.value_or(Theme::getBorderRadius()); }

    // The painted track/fill's own width, independent of the component's full (layout-driven)
    // width - the pill is centered within whatever width the component is actually given.
    void setSliderWidth(float width) { _sliderWidth = width; repaint(); }
    [[nodiscard]] float getSliderWidth() const { return _sliderWidth; }

    void setFontSize(Theme::FontSize size) { _fontSizeOverride = size; lookAndFeelChanged(); }
    void resetFontSize() { _fontSizeOverride = std::nullopt; lookAndFeelChanged(); }
    [[nodiscard]] Theme::FontSize getFontSize() const { return _fontSizeOverride.value_or(Theme::PARAGRAPH); }

    void setFontWeight(Theme::FontWeight weight) { _fontWeightOverride = weight; lookAndFeelChanged(); }
    void resetFontWeight() { _fontWeightOverride = std::nullopt; lookAndFeelChanged(); }
    [[nodiscard]] Theme::FontWeight getFontWeight() const { return _fontWeightOverride.value_or(Theme::REGULAR); }

    void setFont(Theme::FontWeight weight, Theme::FontSize size) { setFontWeight(weight); setFontSize(size); }
    void resetFont() { resetFontWeight(); resetFontSize(); }

private:
    float _sliderWidth = 16.0f;
    std::optional<float> _borderRadiusOverride = std::nullopt;
    std::optional<float> _thumbBorderRadiusOverride = std::nullopt;
    std::optional<Theme::FontSize> _fontSizeOverride = std::nullopt;
    std::optional<Theme::FontWeight> _fontWeightOverride = std::nullopt;

    laf::VerticalSlider _lookAndFeel { *this };
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _attachment;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticalSlider)
};

}
