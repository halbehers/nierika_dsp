#include "../../../include/gui/laf/Dial.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/EmbeddedFonts.h"

namespace nierika::gui::laf
{

namespace
{
    // Mirrors JUCE's own LookAndFeel_V2::SliderLabelComp (the default Label createSliderTextBox
    // normally returns), plus a mouseDoubleClick override that opens the editor unconditionally -
    // see the declaration comment on Dial::createSliderTextBox for why that can't be done through
    // Label's own editOnDoubleClick flag here.
    class DialValueLabel final : public juce::Label
    {
    public:
        DialValueLabel(): juce::Label({}, {}) {}

        void mouseDoubleClick(const juce::MouseEvent&) override
        {
            if (isEnabled())
                showEditor();
        }

        void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails&) override {}

        // drawLabel() switches between the name and the live value based on isMouseOver(true)/
        // isMouseButtonDown(true) - but that's only ever re-evaluated when this label actually
        // repaints. juce::Slider sets setRepaintsOnMouseActivity(true) on itself in its own
        // constructor, which covers hovering the dial proper, but this label is a separate child
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
        // Dial::createSliderTextBox) but never touches justification, so the TextEditor defaults
        // to left-aligned - centred to match drawLabel()'s own static-text rendering.
        juce::TextEditor* createEditorComponent() override
        {
            auto* editor = juce::Label::createEditorComponent();
            editor->setJustification(juce::Justification::centred);
            return editor;
        }
    };
}

Dial::Dial(const element::Dial& parent):
    _parent(parent)
{
}

void Dial::setShortLabel(const juce::String& shortLabel)
{
    _shortLabel = shortLabel;
}

float Dial::computeOuterRadius(float boundsWidth, float boundsHeight) const
{
    // Same formula as drawRotarySlider's own (unaffected-by-gap) sizing, kept in sync by hand
    // rather than shared - see the .h declaration comment.
    constexpr float sizeScalar = 0.7f;

    float maxSize = std::min(boundsWidth, boundsHeight - Theme::getFontSizeInPixels(_parent.getFontSize()) - 9.f);
    maxSize = Theme::resolveDialHeight(_parent.getHeightType(), maxSize) * sizeScalar;

    if (_parent.getHeightType() == Theme::DialHeightType::AUTO)
        maxSize -= boundsWidth / 14.5f;

    const float baseLineWidth = maxSize / 16.f;
    const float lineWidth = std::max(baseLineWidth, 2.f);
    const float dialRadius = std::max(maxSize - 4.0f * lineWidth, 9.0f);
    constexpr float scale = 2.0f;
    const float arcRadius = dialRadius + lineWidth * scale;

    // The arc is stroked with thickness lineWidth centred on arcRadius, so its true visible edge
    // extends lineWidth/2 further out than arcRadius itself.
    return arcRadius + lineWidth * 0.5f;
}

juce::Slider::SliderLayout Dial::getSliderLayout(juce::Slider& slider)
{
    auto layout = LookAndFeel_V4::getSliderLayout(slider);

    if (!_parent.hasLabelGapOverride())
        return layout;

    const auto dialBounds = layout.sliderBounds.toFloat();
    const auto ringBottom = dialBounds.getCentreY() + computeOuterRadius(dialBounds.getWidth(), dialBounds.getHeight());

    layout.textBoxBounds.setY(juce::roundToInt(ringBottom + _parent.getLabelGap()));
    return layout;
}

juce::Label* Dial::createSliderTextBox(juce::Slider& slider)
{
    (void) slider;

    // Slider::textBoxXXXColourId are never configured anywhere for Dial, so reading them (as
    // LookAndFeel_V2::createSliderTextBox, the stock factory this replaces, would) only pulls in
    // JUCE's unthemed defaults - a visible box/outline instead of a bare inline editor matching
    // drawLabel()'s own custom-painted text. Go straight to Theme instead.
    auto* label = new DialValueLabel();

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

void Dial::drawRotarySlider
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
)
{
    _sliderName = slider.getName();
    
    /** Define color variables for customization. */
    const juce::Colour outlineColor = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const juce::Colour fillColor = slider.findColour(juce::Slider::rotarySliderFillColourId);
    const juce::Colour disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED).asJuce();
    const juce::Colour transparentColor = Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce();
    const juce::Colour whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();

    auto dialBounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto centre = dialBounds.getCentre();
    float sizeScalar = 0.7f;

    // Unaffected by getLabelGap() - the dial's own size/position is always "as it is" (see
    // getSliderLayout(), which is what actually implements a fixed label gap by moving the label
    // instead of touching this).
    float maxSize = std::min(dialBounds.getWidth(), (dialBounds.getHeight() - Theme::getFontSizeInPixels(_parent.getFontSize()) - 9.f));
    maxSize = Theme::resolveDialHeight(_parent.getHeightType(), static_cast<float>(maxSize)) * sizeScalar;

    /** Dot color*/
    g.setColour(slider.isEnabled() ? whiteColor : transparentColor);

    if (_parent.getHeightType() == Theme::DialHeightType::AUTO)
        maxSize -= static_cast<float>(width) / 14.5f;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    const float baseLineWidth = maxSize / 16.f;
    const float lineWidth = std::max(baseLineWidth, 2.f);//2.f;
    auto dialRadius = std::max(maxSize - 4.0f * lineWidth, 9.0f);

    {
        juce::Graphics::ScopedSaveState saved(g);
        if (slider.isEnabled())
        {
            juce::ColourGradient fillGradient
            (
                transparentColor,
                centre.getX() + lineWidth * 0.9f,
                centre.getY() - lineWidth * 4.0f,
                transparentColor,
                centre.getX() + dialRadius,
                centre.getY() + dialRadius,
                true
            );
            
            /** Dial center color gradient*/
            g.setGradientFill(fillGradient);
        }
        
        g.fillEllipse(centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * 2.0f, dialRadius * 2.0f);
    }
    //dialRadius = std:: max (dialRadius - 4.0f, 10.0f);

    /** Track thickness*/
    auto scale = 2.0f;
    auto arcRadius = dialRadius + lineWidth * scale;//fullRadius - lineWidth * 2.25;

    juce::Path backgroundArc;
    backgroundArc.addCentredArc
    (
        centre.getX(),
        centre.getY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        rotaryEndAngle,
        true
     );

    /** Dial fill track color*/
    g.setColour(outlineColor);
    g.strokePath(backgroundArc, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    /** Dial outline color*/
    g.setColour(slider.isEnabled() ? whiteColor : disabledColor);
    
    /** Dial outline thickness*/
    dialRadius -= lineWidth;
    g.drawEllipse(centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * scale, dialRadius * scale, std::max(baseLineWidth, 1.5f));
    
    bool isMouseOver = slider.isMouseOver() || slider.isMouseButtonDown();
    
    if (_shortLabel != "" && isMouseOver) {
        const juce::Font font(EmbeddedFonts::getBold()
                              .withHeight(11));

        g.setColour(disabledColor);
        g.setFont(font);
        g.drawText(_shortLabel, static_cast<int>(centre.getX()) - 5, static_cast<int>(centre.getY()) - 6, 10, 12, juce::Justification::centred);
    }

    /** Fill Math*/
    juce::Path dialValueTrack;
    dialValueTrack.addCentredArc
    (
        centre.getX(),
        centre.getY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        toAngle,
        true
     );

    /** Value track fill color*/
    g.setColour(slider.isEnabled() ? fillColor : disabledColor);
    g.strokePath(dialValueTrack, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    /** Dial tick color*/
    g.setColour(slider.isEnabled() ? whiteColor : disabledColor);
    juce::Path dialTick;
    dialTick.startNewSubPath(centre.getPointOnCircumference(dialRadius - lineWidth, toAngle));
    
    /** Dial tick length*/
    dialTick.lineTo(centre.getPointOnCircumference((dialRadius - lineWidth) * 0.6f, toAngle));
    
    /** Dial tick thickness*/
    g.strokePath(dialTick, juce::PathStrokeType(lineWidth * 0.75f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    _sliderWidth = static_cast<float>(slider.getWidth());
}

void Dial::drawLabel (juce::Graphics& g, juce::Label& label)
{
    if (_labelVisibility == HIDDEN) return;

    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    label.setEditable(false);

    const juce::Colour disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED).asJuce();
    const juce::Colour transparentColor = Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce();
    
    if (!label.isBeingEdited())
    {
        const juce::Colour labelColor = label.isEnabled() ? label.findColour (juce::Label::textColourId) : disabledColor;
        const juce::Font font(Theme::newFont(_parent.getFontWeight(), _parent.getFontSize()));

        g.setColour(labelColor);
        g.setFont(font);

        const juce::Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        juce::String labelText;
        if (juce::Component* parentComponent = label.getParentComponent())
        {
            if (auto* slider = dynamic_cast<juce::Slider*>(parentComponent))
            {
                // includeChildren=true - otherwise hovering the value label itself (a child of the
                // slider, not the slider directly) doesn't count, and it falls back to the name.
                const bool isMouseOver = slider->isMouseOver(true) || slider->isMouseButtonDown(true);
                
                // Get the slider value and suffix
                const double value = slider->getValue();
                
                juce::String suffix = slider->getTextValueSuffix();

                switch (_labelVisibility)
                {
                    case HIDDEN:
                        labelText = "";
                        break;
                    case LABEL_ONLY:
                        labelText = slider->getName();
                        break;
                    case VALUE_ONLY:
                        labelText = slider->getTextFromValue(value);
                        break;
                    case VISIBLE:
                    default:
                        labelText = isMouseOver ? slider->getTextFromValue(value) : slider->getName();
                        break;
                }
            }
        }
        
        g.drawFittedText(labelText, textArea, label.getJustificationType(),
                          juce::jmax (1, static_cast<int>(static_cast<float>(textArea.getHeight()) / font.getHeight())), label.getMinimumHorizontalScale());

        g.setColour(labelColor);
    }
    
    else if (label.isEnabled())
    {
        g.setColour(transparentColor);
    }
}

void Dial::setLabelVisibility(LabelVisibility visibility)
{
    _labelVisibility = visibility;
}

}
