#pragma once

#include "../Theme.h"

namespace nierika::gui::element
{

class Level: public juce::Component
{
public:
    Level() = default;
    ~Level() override = default;
    
    void paint(juce::Graphics&) override;

    void setDbLevel(float dbLevel);

private:
    float _dbLevel = -60.f;
    enum State
    {
        DANGER,
        WARNING,
        DEFAULT
    };
    State _state = DEFAULT;
    const std::unordered_map<State, juce::Colour> _stateToColor {
        { DANGER, Theme::getInstance().getColor(Theme::ThemeColor::DANGER).asJuce() },
        { WARNING, Theme::getInstance().getColor(Theme::ThemeColor::WARNING).asJuce() },
        { DEFAULT, Theme::getInstance().getColor(Theme::ThemeColor::ACCENT).asJuce() }
    };
    
    static State computeState(float dbLevel);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Level)
};

}
