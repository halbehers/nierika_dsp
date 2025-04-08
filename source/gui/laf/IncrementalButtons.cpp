#include "../../../include/gui/laf/IncrementalButtons.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/Helpers.h"
#include "../../../include/gui/element/IncrementalButton.h"

namespace ui::laf
{

IncrementalButtons::IncrementalButtons()
{
}

IncrementalButtons::~IncrementalButtons()
{
}

juce::Button* IncrementalButtons::createSliderButton(juce::Slider& slider, bool isIncrement)
{
    if (isIncrement) return new ui::element::IncrementalButton(ui::element::IncrementalButton::Type::PLUS);
    
    return new ui::element::IncrementalButton(ui::element::IncrementalButton::Type::MINUS);
}

}
