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
    const auto outlineColor = slider.findColour (juce::Slider::rotarySliderOutlineColourId);
    const auto fillColor = slider.findColour(juce::Slider::rotarySliderFillColourId);
    const auto disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED).asJuce();
    const auto transparentColor = Theme::getInstance().getColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    const auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();

    auto dialBounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto centre = dialBounds.getCentre();
    float sizeScalar = 0.7f;
    auto fullRadius = juce::jmin(dialBounds.getWidth() * sizeScalar, dialBounds.getHeight() * sizeScalar);
    
    /** Dot color*/
    g.setColour(slider.isEnabled() ? whiteColor : transparentColor);
    centre = dialBounds.getCentre();

    /** Draw dots */
    /** How many dots to draw, works well as num dial intervals + 1 for small ranges, e.g. [0 - 10]*/
//    for (int i = 0; i < 11; ++i)
//    {
//        auto dotSize = width * 0.025;
//
//        /** IF you change the number of dots, do i / (num dots - 1) */
//        const auto angle = juce::jmap (i / 10.0f, rotaryStartAngle, rotaryEndAngle);
//
//        /** Dot distance from slider center */
//        const auto point = centre.getPointOnCircumference (fullRadius - width * 0.06f, angle);
//
//        /** Dot thickness*/
//        g.fillEllipse (point.getX() - 3, point.getY() - 3, dotSize, dotSize);
//    }
        
    fullRadius -= width / 14.5;

    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    float lineWidthMultiplier = width * 0.015f;
    auto lineWidth = std::max(juce::jmin(lineWidthMultiplier, fullRadius * 0.5f), 1.f);
    auto dialRadius = std:: max(fullRadius - 4.0f * lineWidth, 9.0f);

    {
        juce::Graphics::ScopedSaveState saved (g);
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
    g.drawEllipse(centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * scale, dialRadius * scale, 1.0f);
    
    bool isMouseOver = slider.isMouseOver() || slider.isMouseButtonDown();
    
    if (_shortLabel != "" && isMouseOver) {
        const juce::Font font(EmbeddedFonts::getBold()
                              .withHeight(11));

        g.setColour(disabledColor);
        g.setFont(font);
        g.drawText(_shortLabel, centre.getX() - 5, centre.getY() - 6, 10, 12, juce::Justification::centred);
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
    g.strokePath(dialTick, juce::PathStrokeType(lineWidth * 0.75, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    _sliderWidth = slider.getWidth();
}

void Dial::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    label.setEditable(false);
    
    juce::Colour disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED).asJuce();
    juce::Colour transparentColor = Theme::getInstance().getColor(Theme::ThemeColor::TRANSPARENT).asJuce();
    
    if (! label.isBeingEdited())
    {
        auto labelColor = label.isEnabled() ? label.findColour (juce::Label::textColourId) : disabledColor;
        const juce::Font font(EmbeddedFonts::getRegular()
                              .withHeight(std::min((float) (juce::jmax(label.getWidth() * 0.18, label.getHeight() * 0.375)), 15.f))
                              );

        g.setColour(labelColor);
        g.setFont(font);

        auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        juce::String labelText;
        if (auto* parentComponent = label.getParentComponent())
        {
            if (auto* slider = dynamic_cast<juce::Slider*>(parentComponent))
            {
                // Check if the mouse is over the slider
                bool isMouseOver = slider->isMouseOver() || slider->isMouseButtonDown();
                
                // Get the slider value and suffix
                float value = slider->getValue();
                
                juce::String suffix = slider->getTextValueSuffix();
                
                // Determine the text to display based on the mouse over state
                if (isMouseOver)
                {
                    labelText = slider->getTextFromValue(value);
                }
                else
                {
                    labelText = slider->getName();
                }
            }
        }
        
        g.drawFittedText(labelText, textArea, label.getJustificationType(),
                          juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())), label.getMinimumHorizontalScale());

        g.setColour(labelColor);
    }
    
    else if (label.isEnabled())
    {
        g.setColour(transparentColor);
    }
}


}
