#pragma once

namespace nierika::gui::layout
{

class Border
{
public:
    Border() = default;
    explicit Border(juce::Colour color, float thickness = 1.f);
    
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] float getThickness() const;
    [[nodiscard]] juce::Colour getColor() const;
    
private:
    juce::Colour _color {};
    float _thickness = 0.f;
};

}
