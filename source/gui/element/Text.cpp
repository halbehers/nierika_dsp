#include "../../../include/gui/element/Text.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Text::Text(const std::string& identifier, const std::string& name, const std::string& text):
    Component(identifier, name)
{
    addAndMakeVisible(_text);
    _text.setText(text, juce::dontSendNotification);
    setColor(Theme::TEXT);
    setJustificationType(juce::Justification::centred);
    setFont();
}

std::string Text::getText() const
{
    return _text.getText().toStdString();
}

void Text::setText(const std::string& text, const bool triggerRepaint)
{
    _text.setText(text, juce::dontSendNotification);

    if (triggerRepaint) repaint();
}

void Text::setFont(juce::Font font)
{
    _text.setFont(font);
}

void Text::setFont(Theme::FontWeight weight, Theme::FontSize size, Theme::FontFamily family)
{
    _fontSize = size;
    _fontWeight = weight;
    _fontFamily = family;
    setFont(Theme::newFont(family, weight, size));
}

void Text::resetFont()
{
    setFont();
}

void Text::setFontWeight(Theme::FontWeight weight)
{
    _fontWeight = weight;
    setFont(weight, _fontSize, _fontFamily);
}

void Text::setFontSize(Theme::FontSize size)
{
    _fontSize = size;
    setFont(_fontWeight, size, _fontFamily);
}

void Text::setFontFamily(Theme::FontFamily family)
{
    _fontFamily = family;
    setFont(_fontWeight, _fontSize, family);
}

void Text::setColor(Theme::ThemeColor color)
{
    _themeColor = color;
    _text.setColour(juce::Label::textColourId, Theme::newColor(color).asJuce());
}

void Text::setColor(juce::Colour color)
{
    _themeColor.reset();
    _text.setColour(juce::Label::textColourId, color);
}

void Text::setJustificationType(juce::Justification justification)
{
    _text.setJustificationType(justification);
}

void Text::paint(juce::Graphics& g)
{
    Component::paint(g);
}

void Text::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (_themeColor.has_value())
        setColor(*_themeColor);

    Component::changeListenerCallback(source);
}

void Text::resized()
{
    Component::resized();
    _text.setBounds(getLocalBounds());
}

}
