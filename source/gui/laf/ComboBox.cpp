#include "../../../include/gui/laf/ComboBox.h"
#include "../../../include/gui/element/ComboBox.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"
#include "../../../include/gui/EmbeddedFonts.h"
#include "../../../include/gui/Icons.h"

namespace nierika::gui::laf
{

ComboBox::ComboBox(const element::ComboBox& parent):
    _parent(parent)
{
    setColour(juce::PopupMenu::backgroundColourId, juce::Colours::transparentBlack);
}

void ComboBox::drawComboBox
(
    juce::Graphics &g,
    int width,
    int height,
    bool isButtonDown,
    int buttonX,
    int buttonY,
    int buttonW,
    int buttonH,
    juce::ComboBox &comboBox
)
{
    (void) isButtonDown;
    (void) buttonX;
    (void) buttonY;
    (void) buttonW;
    (void) buttonH;
    const auto textColor = Theme::newColor(Theme::ThemeColor::TEXT);
    const auto borderColor = Theme::newColor(Theme::ThemeColor::BORDER);
    const auto backgroundColor = Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND);
    const auto disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED);

    const auto color = comboBox.isEnabled() ? backgroundColor : disabledColor;

    g.setColour(comboBox.isMouseOver() && comboBox.isEnabled() ? color.asJuce() : color.asJuce());

    g.fillRoundedRectangle(0.0, 0.0, width, height, 4.0);

    constexpr int arrowSize = 16;

    helpers::drawFromSVG(g, Icons::getArrowDown(), textColor.asHexString(), width - arrowSize / 2 - 24, height / 2 - arrowSize / 2, arrowSize, arrowSize, juce::AffineTransform());
}

void ComboBox::drawPopupMenuItem
(
    juce::Graphics& g,
    const juce::Rectangle<int>& area,
    bool isSeparator,
    bool isActive,
    bool isHighlighted,
    bool isTicked,
    bool hasSubMenu,
    const juce::String& text,
    const juce::String& shortcutKeyText,
    const juce::Drawable* icon,
    const juce::Colour* textColour
)
{
    (void) isSeparator;
    (void) isActive;
    (void) hasSubMenu;
    (void) shortcutKeyText;
    (void) icon;
    (void) textColour;

    if (isTicked)
    {
        g.setColour(Theme::newColor(Theme::ThemeColor::ACCENT).asJuce());
    }
    else if (isHighlighted)
    {
        g.setColour(Theme::newColor(Theme::ThemeColor::TEXT).asJuce().withAlpha(0.1f));
    }
    else
    {
        g.setColour(Theme::newColor(Theme::ThemeColor::TRANSPARENT).asJuce());
    }

    const int margin = getPopupMenuBorderSize();

    const auto areaWithMargin = area.withX(area.getX() + margin).withWidth(area.getWidth() - (margin * 2)).reduced(2);

    g.fillRoundedRectangle(areaWithMargin.toFloat(), 4.0f);

    if (isTicked && _parent.hasInvertedTextColorOnSelected())
        g.setColour(Theme::newColor(Theme::ThemeColor::INVERTED_TEXT).asJuce());
    else
        g.setColour(Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
    g.setFont(Theme::newFont(Theme::REGULAR, Theme::CAPTION));
    g.drawFittedText(text, areaWithMargin.reduced(4).withX(areaWithMargin.getX() + 12), juce::Justification::centredLeft, 1);
}

void ComboBox::drawPopupMenuBackground
(
    juce::Graphics& g,
	int width,
	int height
)
{
    const juce::Rectangle<float> bounds(0.f, 0.f, (float) width, (float) height);

    g.setColour(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce());
    g.fillRoundedRectangle(bounds, 8.0f);

    g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce());
    g.drawRoundedRectangle(bounds.reduced(0.5f), 8.0f, 1.0f);
}

juce::Font ComboBox::getComboBoxFont(juce::ComboBox& comboBox)
{
    (void) comboBox;
    return Theme::newFont(Theme::REGULAR, Theme::CAPTION);
}

juce::Font ComboBox::getPopupMenuFont()
{
    return Theme::newFont(Theme::REGULAR, Theme::CAPTION);
}

int ComboBox::getPopupMenuBorderSize()
{
    return 12;
}

int ComboBox::getMenuWindowFlags()
{
    return 0; // no native drop shadow - see header comment
}

void ComboBox::getIdealPopupMenuItemSize
(
    const juce::String& text,
	bool isSeparator,
	int standardMenuItemHeight,
	int& idealWidth,
	int& idealHeight
)
{
    idealHeight = standardMenuItemHeight + 8;
}

void ComboBox::positionComboBoxText(juce::ComboBox& comboBox, juce::Label& labelToPosition)
{
    constexpr int top = 0;
    constexpr int left = 8;

    labelToPosition.setBounds(left, top, comboBox.getWidth() - left, comboBox.getHeight() - top);
}

juce::MouseCursor ComboBox::getMouseCursorFor(juce::Component& component)
{
    (void) component;
    return juce::MouseCursor::PointingHandCursor;
}

}
