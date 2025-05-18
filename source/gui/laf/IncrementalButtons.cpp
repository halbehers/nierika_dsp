#include "../../../include/gui/laf/IncrementalButtons.h"
#include "../../../include/gui/element/IncrementalButton.h"

namespace nierika::gui::laf
{

juce::Button* IncrementalButtons::createSliderButton(juce::Slider& slider, bool isIncrement)
{
    if (isIncrement) return new element::IncrementalButton(element::IncrementalButton::Type::PLUS);
    
    return new element::IncrementalButton(element::IncrementalButton::Type::MINUS);
}

}
