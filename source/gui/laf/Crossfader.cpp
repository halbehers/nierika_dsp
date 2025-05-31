#include "../../../include/gui/laf/Crossfader.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

void Crossfader::drawLinearSlider(
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
    (void) minSliderPos;
    (void) maxSliderPos;
    (void) style;

    slider.setTextBoxIsEditable(false);

    const juce::Colour disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED).asJuce();
    const juce::Colour trackColor = Theme::getInstance().getColor(Theme::ThemeColor::LIGHT_SHADE).asJuce().withAlpha(.2f);
    const juce::Colour thumbColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    constexpr int trackSize = 2;
    constexpr int thumbSize = 10;

    g.setColour(trackColor);
    g.fillRoundedRectangle(x, y + height / 2 - trackSize / 2, width, trackSize, 1.f);

    g.setColour(slider.isEnabled() ? thumbColor : disabledColor);
    const int thumbX = std::min(x + static_cast<int>(sliderPos) - thumbSize / 2, x + width - thumbSize - 2);
    g.fillRoundedRectangle(thumbX + 3, y - 1, thumbSize, height + 2, 4.f);
}

void Crossfader::drawLabel(juce::Graphics& g, juce::Label& label)
{
    (void) g;
    (void) label;
}

}
