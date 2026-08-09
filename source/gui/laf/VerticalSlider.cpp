#include "../../../include/gui/laf/VerticalSlider.h"
#include "../../../include/gui/element/VerticalSlider.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

namespace
{
    // Mirrors JUCE's own LookAndFeel_V2::SliderLabelComp (the default Label createSliderTextBox
    // normally returns), plus a mouseDoubleClick override that opens the editor unconditionally -
    // see the declaration comment on VerticalSlider::createSliderTextBox for why that can't be
    // done through Label's own editOnDoubleClick flag here. Same class as laf::Dial's
    // DialValueLabel, duplicated rather than shared since each LookAndFeel in this module owns its
    // painting/interaction logic independently.
    class VerticalSliderValueLabel final : public juce::Label
    {
    public:
        VerticalSliderValueLabel(): juce::Label({}, {}) {}

        void mouseDoubleClick(const juce::MouseEvent&) override
        {
            if (isEnabled())
                showEditor();
        }

        void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override {}

        // drawLabel() switches between the name and the live value based on isMouseOver(true)/
        // isMouseButtonDown(true) - but that's only ever re-evaluated when this label actually
        // repaints. juce::Slider sets setRepaintsOnMouseActivity(true) on itself in its own
        // constructor, which covers hovering the slider proper, but this label is a separate child
        // component the slider creates - the flag isn't (and can't be) inherited, so entering or
        // leaving specifically this label's own bounds needs its own explicit repaint, or the
        // last-painted text (often the value, since that's what shows while hovering) sticks
        // around after the mouse has actually left.
        void mouseEnter(const juce::MouseEvent&) override { repaint(); }
        void mouseExit(const juce::MouseEvent&) override { repaint(); }

        std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override
        {
            return createIgnoredAccessibilityHandler(*this);
        }

        // Label::createEditorComponent() applies our font (see setFont() call in
        // VerticalSlider::createSliderTextBox) but never touches justification, so the TextEditor
        // defaults to left-aligned - centred to match drawLabel()'s own static-text rendering.
        juce::TextEditor* createEditorComponent() override
        {
            auto* editor = juce::Label::createEditorComponent();
            editor->setJustification(juce::Justification::centred);
            return editor;
        }
    };
}

VerticalSlider::VerticalSlider(const element::VerticalSlider& parent):
    _parent(parent)
{
}

juce::Label* VerticalSlider::createSliderTextBox(juce::Slider& slider)
{
    (void) slider;

    // Slider::textBoxXXXColourId are never configured anywhere for VerticalSlider, so reading them
    // (as LookAndFeel_V2::createSliderTextBox, the stock factory this replaces, would) only pulls
    // in JUCE's unthemed defaults - a visible box/outline instead of a bare inline editor matching
    // drawLabel()'s own custom-painted text. Go straight to Theme instead.
    auto* label = new VerticalSliderValueLabel();

    const auto transparentColor = Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce();
    const auto textColor = Theme::newColor(Theme::ThemeColor::TEXT).asJuce();

    label->setJustificationType(juce::Justification::centred);
    label->setKeyboardType(juce::TextInputTarget::decimalKeyboard);
    label->setFont(Theme::newFont(_parent.getFontWeight(), _parent.getFontSize()));

    label->setColour(juce::Label::textColourId, textColor);
    label->setColour(juce::Label::backgroundColourId, transparentColor);
    label->setColour(juce::Label::outlineColourId, transparentColor);
    label->setColour(juce::TextEditor::textColourId, textColor);
    label->setColour(juce::TextEditor::backgroundColourId, transparentColor);
    label->setColour(juce::TextEditor::outlineColourId, transparentColor);
    label->setColour(juce::TextEditor::focusedOutlineColourId, transparentColor);

    return label;
}

void VerticalSlider::drawLinearSlider(
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
)
{
    juce::ignoreUnused(minSliderPos, maxSliderPos, style);

    // The track/fill paints at the configured slider width, centered within whatever width the
    // component was actually given - sliderPos itself is a pure Y coordinate (vertical style), so
    // narrowing the horizontal bounds doesn't affect its already-computed value/position mapping.
    const auto fullBounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
    const auto bounds = fullBounds.withSizeKeepingCentre(juce::jmin(_parent.getSliderWidth(), fullBounds.getWidth()), fullBounds.getHeight());

    juce::Path trackPath;
    trackPath.addRoundedRectangle(bounds, _parent.getBorderRadius());

    g.setColour(slider.findColour(juce::Slider::trackColourId));
    g.fillPath(trackPath);

    // The thumb (filled value bar) is its own independently-rounded rectangle, from sliderPos (the
    // current value's Y position) down to the bottom of the bounds - not clipped against the
    // track's own path, so its corner radius is genuinely independent of the track's.
    juce::Path thumbPath;
    thumbPath.addRoundedRectangle(bounds.withTop(sliderPos), _parent.getThumbBorderRadius());

    const auto disabledColour = Theme::newColor(Theme::ThemeColor::DISABLED).asJuce();
    g.setColour(slider.isEnabled() ? slider.findColour(juce::Slider::thumbColourId) : disabledColour);
    g.fillPath(thumbPath);
}

void VerticalSlider::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    label.setEditable(false);

    if (label.isBeingEdited())
        return;

    const auto disabledColour = Theme::newColor(Theme::ThemeColor::DISABLED).asJuce();
    const auto labelColour = label.isEnabled() ? label.findColour(juce::Label::textColourId) : disabledColour;
    const auto font = Theme::newFont(_parent.getFontWeight(), _parent.getFontSize());

    g.setColour(labelColour);
    g.setFont(font);

    const auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

    juce::String labelText;
    if (auto* slider = dynamic_cast<juce::Slider*>(label.getParentComponent()))
    {
        // Read fresh every repaint (JUCE already repaints on mouse enter/exit) - no cached hover
        // state, same mechanism as laf::Dial::drawLabel. includeChildren=true - otherwise hovering
        // the value label itself (a child of the slider, not the slider directly) doesn't count,
        // and it falls back to the name.
        const bool isMouseOver = slider->isMouseOver(true) || slider->isMouseButtonDown(true);
        labelText = isMouseOver ? slider->getTextFromValue(slider->getValue()) : slider->getName();
    }

    g.drawFittedText(labelText, textArea, label.getJustificationType(),
                      juce::jmax(1, static_cast<int>(static_cast<float>(textArea.getHeight()) / font.getHeight())), label.getMinimumHorizontalScale());
}

}
