#include "../../../include/gui/laf/ComboBox.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"
#include "../../../include/gui/EmbeddedFonts.h"
#include "../../../include/gui/Icons.h"

namespace nierika::gui::laf
{

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
    const auto whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE);
    const auto disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED);

    const auto color = comboBox.isEnabled() ? whiteColor : disabledColor;

    g.setColour(comboBox.isMouseOver() && comboBox.isEnabled() ? color.asJuce().withAlpha(0.1f) : color.asJuce().withAlpha(0.2f));

    g.fillRoundedRectangle(0.0, 0.0, width, height, 4.0);
    
    g.setColour(color.asJuce().withAlpha(0.6f));
    g.setFont(Theme::newFont(Theme::REGULAR, Theme::HEADING));
    g.drawRoundedRectangle(0.0, 0.0, width, height, 4.0, 1.0);

    constexpr int arrowSize = 16;
    
    helpers::drawFromSVG(g, Icons::getArrowDown(), color.asHexString(), width - arrowSize / 2 - 12, height / 2 - arrowSize / 2, arrowSize, arrowSize, juce::AffineTransform());
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
        g.setColour(Theme::newColor(Theme::ThemeColor::ACCENT).asJuce().darker().withAlpha(0.6f));
    }
    else if (isHighlighted)
    {
        g.setColour(Theme::newColor(Theme::ThemeColor::ACCENT).asJuce().darker().withAlpha(0.6f));
    }
    else
    {
        g.setColour(Theme::newColor(Theme::ThemeColor::PRIMARY).asJuce().darker().withAlpha(0.6f));
    }
    g.fillAll();
    
    g.setColour(Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
    g.setFont(Theme::newFont(Theme::REGULAR, Theme::CAPTION));
    g.drawFittedText(text, area.reduced(4.0), juce::Justification::left, 1);
}

juce::Font ComboBox::getComboBoxFont(juce::ComboBox& comboBox)
{
    (void) comboBox;
    return Theme::newFont(Theme::REGULAR, Theme::HEADING);
}

}
