#pragma once

#include "../Theme.h"

namespace nierika::gui::laf
{

class ComboBox : public juce::LookAndFeel_V4
{
public:
    ComboBox();
    ~ComboBox() override;

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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBox)
};

}
