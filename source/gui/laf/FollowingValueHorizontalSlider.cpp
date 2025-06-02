#include "../../../include/gui/laf/FollowingValueHorizontalSlider.h"

namespace nierika::gui::laf
{

FollowingValueHorizontalSlider::FollowingValueHorizontalSlider(const std::string& unit, int gap):
    HorizontalSlider(),
    _unit(unit),
    _gap(gap)
{
}

void FollowingValueHorizontalSlider::drawLinearSlider(
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
)
{
    HorizontalSlider::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);

    (void) minSliderPos;
    (void) maxSliderPos;
    (void) style;

    // FIXME: find out why mouse events are not coming in.
    // if (!slider.isMouseOverOrDragging()) return;

    const int thumbSize = computeThumbSize(height);
    const int thumbX = computeThumbX(x, width, sliderPos, thumbSize);
    int valueX = thumbX + thumbSize + _gap;
    const std::string valueText = Formatter::formatDouble(slider.getValue()) + " " + _unit;

    const int valueWidth = _valueWidth == -1 ? static_cast<int>(valueText.size()) * 5 : _valueWidth;

    if (width - valueX - valueWidth < 0) {
        valueX = thumbX - _gap + 6 - valueWidth;
    }

    g.setFont(Theme::newFont(_fontWeight, _fontSize));
    g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());

    if (slider.isMouseOver())
        g.drawFittedText(valueText, valueX, y + height - static_cast<int>(Theme::getFontSizeInPixels(_fontSize)), valueWidth, 30, juce::Justification::topLeft, 1);
}

void FollowingValueHorizontalSlider::setUnit(const std::string& unit)
{
    _unit = unit;
}

void FollowingValueHorizontalSlider::setGap(int gap)
{
    _gap = gap;
}

void FollowingValueHorizontalSlider::setFontSize(Theme::FontSize fontSize)
{
    _fontSize = fontSize;
}

void FollowingValueHorizontalSlider::setFontWeight(Theme::FontWeight fontWeight)
{
    _fontWeight = fontWeight;
}

void FollowingValueHorizontalSlider::setFont(Theme::FontSize fontSize, Theme::FontWeight fontWeight)
{
    _fontSize = fontSize;
    _fontWeight = fontWeight;
}

void FollowingValueHorizontalSlider::setValueWidth(int width)
{
    _valueWidth = width;
}

}
