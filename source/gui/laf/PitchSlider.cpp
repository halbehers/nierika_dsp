#include "../../../include/gui/laf/PitchSlider.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

void PitchSlider::drawLinearSlider(
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
    const juce::Colour thumbColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.4f);
    const juce::Colour trackColor = Theme::newColor(Theme::ThemeColor::LIGHT_SHADE).asJuce().withAlpha(0.2f);
    constexpr int thumbSize = 5;

    g.setColour(trackColor);
    g.fillRect(x - 1, y - 1, static_cast<int>(sliderPos) - thumbSize / 2 + 1, height + 2);

    g.setColour(slider.isEnabled() ? thumbColor : disabledColor);
    const int thumbX = std::min(x + static_cast<int>(sliderPos) - thumbSize / 2, x + width - thumbSize);
    g.fillRect(thumbX, y - 1, thumbSize, height + 2);
}

void PitchSlider::drawLabel(juce::Graphics& g, juce::Label& label)
{
    (void) g;
    (void) label;
}

}
