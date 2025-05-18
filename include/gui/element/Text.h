#pragma once

#include <string>

#include "../Component.h"
#include "../Theme.h"

namespace nierika::gui::element
{

class Text: public Component
{
public:
    explicit Text(const std::string& identifier, const std::string& name = "", const std::string& text = "");
    ~Text() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    std::string getText() const;
    void setText(const std::string& text, bool triggerRepaint = true);
    void setFont(juce::Font);
    void setFont(Theme::FontWeight weight = Theme::REGULAR, Theme::FontSize size = Theme::PARAGRAPH);
    void resetFont();
    void setFontWeight(Theme::FontWeight weight);
    void setFontSize(Theme::FontSize size);
    void setColor(Theme::ThemeColor color);
    void setColor(juce::Colour color);
    void setJustificationType(juce::Justification justification);

private:
    juce::Label _text {};
    Theme::FontSize _fontSize = Theme::PARAGRAPH;
    Theme::FontWeight _fontWeight = Theme::REGULAR;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Text)
};

}
