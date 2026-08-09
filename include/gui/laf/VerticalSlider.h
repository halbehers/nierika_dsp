#pragma once

namespace nierika::gui::element { class VerticalSlider; }

namespace nierika::gui::laf
{

class VerticalSlider final : public juce::LookAndFeel_V4
{
public:
    explicit VerticalSlider(const element::VerticalSlider& parent);
    ~VerticalSlider() override = default;

    void drawLinearSlider(
        juce::Graphics& g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        juce::Slider::SliderStyle style,
        juce::Slider& slider
    ) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    // Swaps in a Label subclass whose double-click opens the editor directly, independent of
    // juce::Label's own editOnSingleClick/editOnDoubleClick flags - drawLabel() above calls
    // label.setEditable(false) unconditionally on every repaint (needed so a stray single click
    // never opens the stock editor over our custom-painted text), which would otherwise also wipe
    // out any editOnDoubleClick flag set through the normal API just as reliably. Same approach as
    // laf::Dial::createSliderTextBox.
    juce::Label* createSliderTextBox(juce::Slider& slider) override;

private:
    const element::VerticalSlider& _parent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticalSlider)
};

}
