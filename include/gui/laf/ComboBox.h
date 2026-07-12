#pragma once

namespace nierika::gui::element { class ComboBox; }

namespace nierika::gui::laf
{

class ComboBox final : public juce::LookAndFeel_V4
{
public:
    explicit ComboBox(const element::ComboBox& parent);
    ~ComboBox() override = default;

    void drawComboBox
    (
        juce::Graphics &,
        int width,
        int height,
        bool isButtonDown,
        int buttonX,
        int buttonY,
        int buttonW,
        int buttonH,
        juce::ComboBox &
     ) override;
    
    void drawPopupMenuItem
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
    ) override;
    
    juce::Font getComboBoxFont(juce::ComboBox &) override;

    juce::Font getPopupMenuFont() override;

    void drawPopupMenuBackground
    (
        juce::Graphics& g,
		int width,
		int height
    ) override;

    int getPopupMenuBorderSize() override;

    void getIdealPopupMenuItemSize
    (
        const juce::String& text,
		bool isSeparator,
		int standardMenuItemHeight,
		int& idealWidth,
		int& idealHeight
    ) override;

    void positionComboBoxText(juce::ComboBox& comboBox, juce::Label& labelToPosition) override;

    juce::MouseCursor getMouseCursorFor(juce::Component& component) override;

protected:
    const element::ComboBox& _parent;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBox)
};

}
