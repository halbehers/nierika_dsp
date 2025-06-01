#include "../../../include/gui/laf/HorizontalSlider.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

void HorizontalSlider::drawLinearSlider(
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
    (void) width;
    (void) minSliderPos;
    (void) maxSliderPos;
    (void) style;

    slider.setTextBoxIsEditable(false);

    const juce::Colour disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED).asJuce();
    const juce::Colour thumbColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    const juce::Colour trackColor = Theme::newColor(Theme::ThemeColor::LIGHT_SHADE).asJuce().withAlpha(0.2f);
    constexpr int trackSize = 2;
    const int thumbSize = computeThumbSize(height);

    g.setColour(trackColor);
    g.fillRoundedRectangle(x, y + height / 2 - trackSize / 2, width, trackSize, 1.f);

    g.setColour(slider.isEnabled() ? thumbColor : disabledColor);
    g.fillRoundedRectangle(computeThumbX(x, width, sliderPos, thumbSize) + 3, y - 1, thumbSize, height + 2, 4.f);
}

int HorizontalSlider::computeThumbSize(int height)
{
    return std::max(height / 5, 5);
}

int HorizontalSlider::computeThumbX(int x, int width, float sliderPos, int thumbSize)
{
    return std::min(x + static_cast<int>(sliderPos) - thumbSize / 2, x + width - thumbSize - 2);
}

void HorizontalSlider::drawLabel(juce::Graphics& g, juce::Label& label)
{
    (void) g;
    (void) label;
}

}
