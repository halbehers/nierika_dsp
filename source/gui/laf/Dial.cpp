#include "../../../include/gui/laf/Dial.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/EmbeddedFonts.h"

namespace nierika::gui::laf
{

void Dial::setShortLabel(const juce::String& shortLabel)
{
    _shortLabel = shortLabel;
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
    const juce::Colour transparentColor = Theme::newColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    const juce::Colour whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();

    auto dialBounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto centre = dialBounds.getCentre();
    float sizeScalar = 0.7f;
    float maxSize = std::min(dialBounds.getWidth() * sizeScalar, (dialBounds.getHeight() - _labelHeight - 9.f) * sizeScalar) ;

    /** Dot color*/
    g.setColour(slider.isEnabled() ? whiteColor : transparentColor);
    centre = dialBounds.getCentre();

    maxSize -= static_cast<float>(width) / 14.5f;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    //float lineWidthMultiplier = maxSize * 0.015f;
    //auto lineWidth = std::max(juce::jmin(lineWidthMultiplier, maxSize * 0.5f), 1.f);
    const float lineWidth = std::max(maxSize / 16.f, 2.f);//2.f;
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
    g.drawEllipse(centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * scale, dialRadius * scale, 1.0f);
    
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
    
    _sliderWidth = slider.getWidth();
}

void Dial::drawLabel (juce::Graphics& g, juce::Label& label)
{
    if (_labelVisibility == HIDDEN) return;

    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    label.setEditable(false);

    const juce::Colour disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED).asJuce();
    const juce::Colour transparentColor = Theme::newColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    
    if (! label.isBeingEdited())
    {
        const juce::Colour labelColor = label.isEnabled() ? label.findColour (juce::Label::textColourId) : disabledColor;
        const juce::Font font(Theme::newFont(Theme::REGULAR, _labelHeight));

        g.setColour(labelColor);
        g.setFont(font);

        const juce::Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        juce::String labelText;
        if (juce::Component* parentComponent = label.getParentComponent())
        {
            if (auto* slider = dynamic_cast<juce::Slider*>(parentComponent))
            {
                // Check if the mouse is over the slider
                const bool isMouseOver = slider->isMouseOver() || slider->isMouseButtonDown();
                
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
