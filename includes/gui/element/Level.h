#pragma once

#include "../Theme.h"

namespace nierika::ui::element
{

class Level: public juce::Component
{
public:
    Level();
    ~Level() override;
    
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
    
    State computeState(float dbLevel);
};

}
