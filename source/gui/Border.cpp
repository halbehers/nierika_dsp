#include "../../include/gui/Border.h"

namespace nierika::gui::layout
{
Border::Border(juce::Colour color, float thickness):
    _color(color),
    _thickness(thickness)
{
}

bool Border::isEmpty() const
{
    return _thickness == 0.f;
}

float Border::getThickness() const
{
    return _thickness;
}

juce::Colour Border::getColor() const
{
    return _color;
}

}
