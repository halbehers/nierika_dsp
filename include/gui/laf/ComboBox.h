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

    // Default LookAndFeel_V2 behaviour adds ComponentPeer::windowHasDropShadow - a native OS
    // drop shadow anchored to the popup's full rectangular window frame, unclipped to the rounded
    // corners drawn in drawPopupMenuBackground. Against a dark scrim that reads as a second, hard
    // rectangular border outside the one we actually draw - disable it entirely.
    int getMenuWindowFlags() override;

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
