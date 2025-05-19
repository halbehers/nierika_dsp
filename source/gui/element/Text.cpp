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

void Text::setFont(Theme::FontWeight weight, Theme::FontSize size)
{
    _fontSize = size;
    _fontWeight = weight;
    setFont(Theme::getInstance().getFont(weight, size));
}

void Text::resetFont()
{
    setFont();
}

void Text::setFontWeight(Theme::FontWeight weight)
{
    _fontWeight = weight;
    setFont(weight, _fontSize);
}

void Text::setFontSize(Theme::FontSize size)
{
    _fontSize = size;
    setFont(_fontWeight, size);
}

void Text::setColor(Theme::ThemeColor color)
{
    _text.setColour(juce::Label::textColourId, Theme::getInstance().getColor(color).asJuce());
}

void Text::setColor(juce::Colour color)
{
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

void Text::resized()
{
    Component::resized();
    _text.setBounds(getLocalBounds());
}

}
