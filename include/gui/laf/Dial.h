#pragma once

namespace nierika::gui::element { class Dial; }

namespace nierika::gui::laf
{

class Dial final : public juce::LookAndFeel_V4
{
public:
    enum LabelVisibility
    {
        VISIBLE,
        LABEL_ONLY,
        VALUE_ONLY,
        HIDDEN
    };

    explicit Dial(const element::Dial& parent);
    ~Dial() override = default;

    void drawRotarySlider
    (
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider &slider
    ) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    // Only overridden to move the label when the parent has a fixed label gap set - the dial's
    // own sliderBounds (and so its size/position, computed in drawRotarySlider) stay exactly what
    // JUCE's own LookAndFeel_V4::getSliderLayout would already produce; only textBoxBounds' Y
    // moves, to land computeOuterRadius() + getLabelGap() pixels below the dial's centre. Falls
    // through to the base implementation entirely when no gap is set.
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;

    // Swaps in a Label subclass whose double-click opens the editor directly, independent of
    // juce::Label's own editOnSingleClick/editOnDoubleClick flags - drawLabel() below calls
    // label.setEditable(false) unconditionally on every repaint (needed so a stray single click
    // never opens the stock editor over our custom-painted text), which would otherwise also wipe
    // out any editOnDoubleClick flag set through the normal API just as reliably.
    juce::Label* createSliderTextBox(juce::Slider& slider) override;

    void setShortLabel(const juce::String& shortLabel);

    void setLabelVisibility(LabelVisibility visibility);

protected:
    const element::Dial& _parent;

private:
    float _sliderWidth = 0.f;
    juce::String _sliderName = "";
    juce::String _shortLabel = "";
    LabelVisibility _labelVisibility = VISIBLE;
    const Theme::FontSize _labelHeight = Theme::PARAGRAPH;

    // Mirrors drawRotarySlider's own sizing formula (maxSize -> lineWidth -> dialRadius ->
    // arcRadius) just far enough to return the radius of the ring it will actually paint - kept
    // as a separate, parallel calculation rather than a shared one drawRotarySlider also calls,
    // so that path stays exactly what it was before getLabelGap() existed ("the dial as it is").
    [[nodiscard]] float computeOuterRadius(float boundsWidth, float boundsHeight) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dial)
};

}
