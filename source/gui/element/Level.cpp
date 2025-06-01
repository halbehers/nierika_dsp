#include "../../../include/gui/element/Level.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

void Level::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.20f));
    g.fillRoundedRectangle(bounds, 2.f);

    const auto scaleY = juce::jmap(_dbLevel, -100.f, 6.f, 0.f, static_cast<float>(getHeight()));

    const std::unordered_map<State, juce::Colour> stateToColor {
        { DANGER, Theme::newColor(Theme::ThemeColor::DANGER).asJuce() },
        { WARNING, Theme::newColor(Theme::ThemeColor::WARNING).asJuce() },
        { DEFAULT, Theme::newColor(Theme::ThemeColor::ACCENT).asJuce() }
    };

    if (isEnabled())
        g.setColour(stateToColor.at(_state));
    else
        g.setColour(Theme::newColor(Theme::ThemeColor::DISABLED).asJuce());
    g.fillRoundedRectangle(bounds.removeFromBottom(scaleY), 2.f);
}

Level::State Level::computeState(float dbLevel)
{
    if (dbLevel > 0.f) return DANGER;
    if (dbLevel > -2.f) return WARNING;

    return DEFAULT;
}

void Level::setDbLevel(float dbLevel)
{
    _state = computeState(dbLevel);
    _dbLevel = juce::jmax(juce::jmin(dbLevel, 6.f), -100.f);
}

}
