#pragma once

#include "HorizontalSlider.h"

namespace nierika::gui::laf
{

class FollowingValueHorizontalSlider final : public HorizontalSlider
{
public:
    FollowingValueHorizontalSlider() = default;
    explicit FollowingValueHorizontalSlider(const std::string& unit, int gap = 8);
    ~FollowingValueHorizontalSlider() override = default;

    void setUnit(const std::string& unit);
    void setGap(int gap);
    void setFontSize(Theme::FontSize fontSize);
    void setFontWeight(Theme::FontWeight fontWeight);
    void setFont(Theme::FontSize fontSize, Theme::FontWeight fontWeight);
    void setValueWidth(int width);

    void drawLinearSlider(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        juce::Slider::SliderStyle style,
        juce::Slider &slider
    ) override;

private:
    std::string _unit;
    int _gap = 8;
    Theme::FontSize _fontSize = Theme::HEADING;
    Theme::FontWeight _fontWeight = Theme::REGULAR;
    int _valueWidth = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FollowingValueHorizontalSlider)
};

}
