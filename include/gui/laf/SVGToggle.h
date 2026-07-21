#pragma once

#include <unordered_map>

#include "../Theme.h"

namespace nierika::gui::element { class SVGToggle; }

namespace nierika::gui::laf
{

class SVGToggle final : public juce::LookAndFeel_V4
{
public:
    explicit SVGToggle(const element::SVGToggle& parent);
    ~SVGToggle() override = default;
    
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

    void drawToggleButton
    (
        juce::Graphics& g,
        juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted,
        bool shouldDrawButtonAsDown
    ) override;
    
    State getState(bool ticked,
                   bool isEnabled,
                   bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown);

private:
    const element::SVGToggle& _parent;
    std::unordered_map<State, Theme::ThemeColor> _stateToThemeColors {
        {
            { SVGToggle::State::DISABLED, Theme::ThemeColor::DISABLED },
            { SVGToggle::State::HIGHLIGHTED, Theme::ThemeColor::LIGHTER_SHADE },
            { SVGToggle::State::DOWN, Theme::ThemeColor::MEDIUM_SHADE },
            { SVGToggle::State::SELECTED, Theme::ThemeColor::EMPTY_SHADE },
            { SVGToggle::State::DEFAULT, Theme::ThemeColor::DISABLED },
        }
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGToggle)

};

}
