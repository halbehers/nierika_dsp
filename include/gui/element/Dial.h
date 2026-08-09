#pragma once

#include "../laf/Dial.h"
#include "../Component.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

class Dial: public Component
{
public:
    enum Size
    {
        SMALL,
        MEDIUM,
        LARGE
    };
    
    Dial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, const std::string& valueSuffix = "");
    Dial(dsp::ParameterManager& parameterManager, const std::string& parameterID, const std::string& valueSuffix = "");
    ~Dial() override;

    void resized() override;
    void setShortLabel(const juce::String& shortLabel);
    void setEnabled(bool isEnabled);

    void setLabelVisibility(laf::Dial::LabelVisibility visibility);

    // Overrides the text shown as this dial's label (below the dial, and on hover shows the
    // value instead) without touching the underlying APVTS parameter's own registered name -
    // that name is what hosts show in their generic automation lane/parameter list, which often
    // wants to stay more fully qualified (e.g. "Filter Cutoff") than what reads well as an
    // on-dial label in a compact UI (e.g. just "Cutoff").
    void setDisplayLabel(const juce::String& label) { _slider.setName(label); }

    // Overrides the accent color used for the dial's value-fill arc (defaults to Theme::ACCENT,
    // applied in setup()) - lets one module use a different accent from the rest of the app
    // without touching every other Dial's default.
    void setAccentColor(Theme::ThemeColor colorId);

    void setHeightType(Theme::DialHeightType type) { _heightType = type; resized(); _slider.lookAndFeelChanged(); }
    [[nodiscard]] Theme::DialHeightType getHeightType() const { return _heightType; }

    void setFontSize(Theme::FontSize size) { _fontSizeOverride = size; _slider.lookAndFeelChanged(); }
    void resetFontSize() { _fontSizeOverride = std::nullopt; _slider.lookAndFeelChanged(); }
    Theme::FontSize getFontSize() const { return _fontSizeOverride.value_or(Theme::PARAGRAPH); }

    void setFontWeight(Theme::FontWeight weight) { _fontWeightOverride = weight; _slider.lookAndFeelChanged(); }
    void resetFontWeight() { _fontWeightOverride = std::nullopt; _slider.lookAndFeelChanged(); }
    Theme::FontWeight getFontWeight() const { return _fontWeightOverride.value_or(Theme::REGULAR); }

    void setFont(Theme::FontWeight weight, Theme::FontSize size) { setFontWeight(weight); setFontSize(size); }
    void resetFont() { resetFontWeight(); resetFontSize(); }

    // Fixed pixel distance between the dial's outer ring and its label, once set - the dial's own
    // size/position is entirely unaffected (still centred in its drawing area exactly as before);
    // laf::Dial::getSliderLayout instead moves the label itself to land this many pixels below
    // wherever the dial's outer ring ends up. Unset (the default) keeps today's behaviour: JUCE's
    // own fixed-height text box positioning, untouched.
    void setLabelGap(float gap) { _labelGapOverride = gap; _slider.lookAndFeelChanged(); }
    void resetLabelGap() { _labelGapOverride = std::nullopt; _slider.lookAndFeelChanged(); }
    [[nodiscard]] float getLabelGap() const { return _labelGapOverride.value_or(9.f); }
    [[nodiscard]] bool hasLabelGapOverride() const { return _labelGapOverride.has_value(); }

protected:
    float _minValue;
    float _maxValue;
    float _defaultValue;
    std::string _valueSuffix;
    Theme::DialHeightType _heightType = Theme::DialHeightType::AUTO;
    std::optional<Theme::FontSize> _fontSizeOverride = std::nullopt;
    std::optional<Theme::FontWeight> _fontWeightOverride = std::nullopt;
    std::optional<float> _labelGapOverride = std::nullopt;

    juce::Slider _slider;
    laf::Dial _lookAndFeel { *this };

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _attachment;

    virtual void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dial)
};

}
