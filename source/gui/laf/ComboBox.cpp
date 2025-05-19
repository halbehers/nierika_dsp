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
    const auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE);
    const auto disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED);

    const auto color = comboBox.isEnabled() ? whiteColor : disabledColor;

    g.setColour(comboBox.isMouseOver() && comboBox.isEnabled() ? color.asJuce().withAlpha(0.1f) : color.asJuce().withAlpha(0.2f));

    g.fillRoundedRectangle(0.0, 0.0, width, height, 4.0);
    
    g.setColour(color.asJuce().withAlpha(0.6f));
    g.setFont(EmbeddedFonts::getRegular().withHeight(14.0));
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
        g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::ACCENT).asJuce().darker().withAlpha(0.6f));
    }
    else if (isHighlighted)
    {
        g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::ACCENT).asJuce().darker().withAlpha(0.6f));
    }
    else
    {
        g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::PRIMARY).asJuce().darker().withAlpha(0.6f));
    }
    g.fillAll();
    
    g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::TEXT).asJuce());
    g.setFont(EmbeddedFonts::getRegular().withHeight(13.0));
    g.drawFittedText(text, area.reduced(4.0), juce::Justification::left, 1);
}

juce::Font ComboBox::getComboBoxFont(juce::ComboBox& comboBox)
{
    (void) comboBox;
    return EmbeddedFonts::getRegular().withHeight(14.0);
}

}
