#pragma once

namespace nierika::gui::layout
{

class Border
{
public:
    Border();
    Border(juce::Colour color, float thickness = 1.f);
    
    bool isEmpty() const;
    float getThickness() const;
    juce::Colour getColor() const;
    
private:
    juce::Colour _color;
    float _thickness = 0.f;
};

}
