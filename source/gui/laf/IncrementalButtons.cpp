#include "../../../includes/gui/laf/IncrementalButtons.h"
#include "../../../includes/gui/Theme.h"
#include "../../../includes/gui/Helpers.h"
#include "../../../includes/gui/element/IncrementalButton.h"

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
