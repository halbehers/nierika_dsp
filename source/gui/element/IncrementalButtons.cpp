namespace nierika::gui::element
{

IncrementalButtons::IncrementalButtons(float minValue, float maxValue, float defaultValue):
    Slider("", minValue, maxValue, defaultValue, "")
{
    setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    setLookAndFeel(&_lookAndFeel);
}

void IncrementalButtons::paint(juce::Graphics& g)
{
    Slider::paint(g);
}

void IncrementalButtons::resized()
{
    Slider::resized();
    setBounds(getX(), getY(), 40, 16);
}

}
