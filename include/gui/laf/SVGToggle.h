#pragma once

#include <unordered_map>

#include "../Theme.h"

namespace nierika::gui::laf
{

class SVGToggle : public juce::LookAndFeel_V4
{
public:
    SVGToggle();
    SVGToggle(const char* svgBinary);
    ~SVGToggle() override;
    
    enum State
    {
        DISABLED,
        HIGHLIGHTED,
        DOWN,
        SELECTED,
        DEFAULT
    };
    
    void drawTickBox
    (
        juce::Graphics& g,
        juce::Component& button,
        float x,
        float y,
        float w,
        float h,
        bool ticked,
        bool isEnabled,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown
     ) override;
    
    State getState(bool ticked,
                   bool isEnabled,
                   bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown);

private:
    const char* _svgBinary;
    std::unordered_map<State, Theme::ThemeColor> _stateToThemeColors {
        {
            { SVGToggle::State::DISABLED, Theme::ThemeColor::DISABLED },
            { SVGToggle::State::HIGHLIGHTED, Theme::ThemeColor::LIGHTER_SHADE },
            { SVGToggle::State::DOWN, Theme::ThemeColor::MEDIUM_SHADE },
            { SVGToggle::State::SELECTED, Theme::ThemeColor::EMPTY_SHADE },
            { SVGToggle::State::DEFAULT, Theme::ThemeColor::DISABLED },
        }
    };

};

}
