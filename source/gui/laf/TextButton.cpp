#include "../../../include/gui/laf/TextButton.h"
#include "../../../include/gui/element/TextButton.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::laf
{

namespace
{
    // Mirrors the corner-based text indent JUCE's LookAndFeel_V2::drawButtonText uses, so an
    // icon (when present) lines up with where the text would otherwise start, and the
    // configurable gap only ever separates the icon from the text, not from the button edge.
    int cornerBasedIndent(int buttonWidth, int buttonHeight, int fontHeight, bool isConnected)
    {
        const int cornerSize = juce::jmin(buttonHeight, buttonWidth) / 2;
        return juce::jmin(fontHeight, 2 + cornerSize / (isConnected ? 4 : 2));
    }
}

TextButton::TextButton(const element::TextButton& parent):
    _parent(parent)
{
}

juce::Font TextButton::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    (void) button;
    (void) buttonHeight;
    return Theme::newFont(_parent.getFontWeight(), _parent.getFontSize());
}

void TextButton::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    (void) backgroundColour;
    (void) shouldDrawButtonAsDown;

    const auto bounds = button.getLocalBounds().toFloat();
    const auto radius = _parent.getBorderRadius();
    const auto baseColour = _parent.isSelected() ? _parent.getSelectedBackgroundColour() : _parent.getBackgroundColour();

    // TabDesign::TAB tabs sit flush edge-to-edge with no gap and are always plain rectangles -
    // including the selected one, which must stay perfectly square. The illusion of it "popping
    // up" instead comes from its unselected neighbour(s) below, whose near top corner gets a
    // concave notch bitten out (see the notch painting below). TabDesign::BUTTON (the default) is
    // unaffected: all four corners round exactly as before.
    const bool isTabDesign = _parent.getTabDesign() == Theme::TabDesign::TAB;
    const bool curveCorners = !isTabDesign;

    juce::Path backgroundPath;
    backgroundPath.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                                        radius, radius, curveCorners, curveCorners, curveCorners, curveCorners);

    // Tabs::reset() disables the selected tab purely to lock its own click/hover behaviour, not
    // to signal an actually-disabled control - it must not fall into the disabled-alpha look,
    // which would wash out its dedicated selected colour.
    if (shouldDrawButtonAsHighlighted)
        g.setColour(_parent.getHoverBackgroundColour());
    else if (!button.isEnabled() && !_parent.isSelected())
        g.setColour(baseColour.withAlpha(0.7f));
    else
        g.setColour(baseColour);

    g.fillPath(backgroundPath);

    const auto borderColour = _parent.isSelected() ? _parent.getSelectedBorderColour() : _parent.getBorderColour();
    if (!borderColour.isTransparent())
    {
        const auto borderBounds = bounds.reduced(0.5f);
        juce::Path borderPath;
        borderPath.addRoundedRectangle(borderBounds.getX(), borderBounds.getY(), borderBounds.getWidth(), borderBounds.getHeight(),
                                        radius, radius, curveCorners, curveCorners, curveCorners, curveCorners);
        g.setColour(borderColour);
        g.strokePath(borderPath, juce::PathStrokeType(1.f));
    }

    if (isTabDesign)
    {
        const auto notchColour = _parent.getNotchColour();
        if (!notchColour.isTransparent())
        {
            // Each notch is the corner sliver a standard rounded-rect corner would normally cut
            // off (same centre/radius as addRoundedRectangle's own corner math) - filling it with
            // the selected tab's colour, on top of this tab's own square fill, is what makes the
            // curve read as flowing out from underneath the selected tab.
            if (_parent.hasLeftNotch())
            {
                juce::Path notch;
                notch.startNewSubPath(bounds.getX(), bounds.getY());
                notch.addArc(bounds.getX(), bounds.getY(), radius * 2.f, radius * 2.f, 0.f, -juce::MathConstants<float>::halfPi, false);
                notch.closeSubPath();
                g.setColour(notchColour);
                g.fillPath(notch);
            }
            if (_parent.hasRightNotch())
            {
                juce::Path notch;
                notch.startNewSubPath(bounds.getRight(), bounds.getY());
                notch.addArc(bounds.getRight() - radius * 2.f, bounds.getY(), radius * 2.f, radius * 2.f, 0.f, juce::MathConstants<float>::halfPi, false);
                notch.closeSubPath();
                g.setColour(notchColour);
                g.fillPath(notch);
            }
        }
    }

    if (!_parent.hasIcon())
        return;

    auto& textButton = static_cast<juce::TextButton&>(button);
    const bool isLeft = _parent.getIconPosition() == element::TextButton::IconPosition::LEFT;
    const int fontHeight = juce::roundToInt(getTextButtonFont(textButton, button.getHeight()).getHeight() * 0.6f);
    const int edgeIndent = cornerBasedIndent(button.getWidth(), button.getHeight(), fontHeight,
                                              isLeft ? textButton.isConnectedOnLeft() : textButton.isConnectedOnRight());

    const float iconSize = _parent.getIconSize();
    const float iconY = (bounds.getHeight() - iconSize) * 0.5f;
    const float iconX = isLeft ? static_cast<float>(edgeIndent) : bounds.getWidth() - static_cast<float>(edgeIndent) - iconSize;

    helpers::drawFromSVG(g, _parent.getIconBinary(), Theme::newColor(_parent.hasInvertedTextColor() ? Theme::ThemeColor::INVERTED_TEXT : Theme::ThemeColor::TEXT).asHexString(),
                          static_cast<int>(iconX), static_cast<int>(iconY),
                          static_cast<int>(iconSize), static_cast<int>(iconSize),
                          juce::AffineTransform());
}

void TextButton::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    (void) shouldDrawButtonAsHighlighted;
    (void) shouldDrawButtonAsDown;

    const juce::Font font(getTextButtonFont(button, button.getHeight()));
    g.setFont(font);
    const auto textColour = _parent.isSelected() ? _parent.getSelectedTextColour() : _parent.getTextColour();
    const bool isDimmed = !button.isEnabled() && !_parent.isSelected();
    g.setColour(textColour.withMultipliedAlpha(isDimmed ? 0.5f : 1.0f));

    const int yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    const int fontHeight = juce::roundToInt(font.getHeight() * 0.6f);

    int leftIndent = cornerBasedIndent(button.getWidth(), button.getHeight(), fontHeight, button.isConnectedOnLeft());
    int rightIndent = cornerBasedIndent(button.getWidth(), button.getHeight(), fontHeight, button.isConnectedOnRight());

    if (_parent.hasIcon())
    {
        // The icon sits at the same base indent the text would otherwise start at (see
        // drawButtonBackground), so the gap here only needs to cover icon size + spacing to text.
        const int iconAndGap = juce::roundToInt(_parent.getIconSize() + _parent.getGap());
        if (_parent.getIconPosition() == element::TextButton::IconPosition::LEFT)
            leftIndent += iconAndGap;
        else
            rightIndent += iconAndGap;
    }

    const int textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
                          leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
                          juce::Justification::centred, 2);
}

}
