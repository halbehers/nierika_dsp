namespace nierika::gui::laf
{

juce::Button* IncrementalButtons::createSliderButton(juce::Slider& slider, bool isIncrement)
{
    (void) slider;

    if (isIncrement) return new element::IncrementalButton(element::IncrementalButton::Type::PLUS);
    
    return new element::IncrementalButton(element::IncrementalButton::Type::MINUS);
}

}
