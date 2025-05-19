namespace nierika::gui::element
{

ComboBox::ComboBox(const juce::String& name)
{
    setLookAndFeel(&_lookAndFeel);
    juce::ComboBox::setName(name);
}

void ComboBox::paint (juce::Graphics& g)
{
    juce::ComboBox::paint(g);
}

void ComboBox::resized()
{
    juce::ComboBox::resized();
}

}
