#include "../../../include/gui/element/Stars.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Stars::Stars(int maxWidth, int maxHeight):
    _width(maxWidth),
    _height(maxHeight)
{
    _glowEffect.setGlowProperties(6.0, Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.3f));
    
    computeSmallStars();
    computeMediumStars();
    computeLargeStars();
}

void Stars::computeSmallStars()
{
    constexpr int density = 48;
    const int xCount = _width / density;
    const int yCount = _height / density;
    
    _smallStarPositions.resize(static_cast<std::size_t>(xCount * yCount));
    
    for (int x = 0; x < xCount; ++x)
    {
        for (int y = 0; y < yCount; ++y)
        {
            float xPosition = std::rand() % _width + 1;
            float yPosition = std::rand() % _height + 1;
            _smallStarPositions.push_back(juce::Point<float>(xPosition, yPosition));
        }
    }
}

void Stars::computeMediumStars()
{
    constexpr int density = 96;
    const int xCount = _width / density;
    const int yCount = _height / density;
    
    _mediumStarPositions.resize(static_cast<std::size_t>(xCount * yCount));
    
    for (int x = 0; x < xCount; ++x)
    {
        for (int y = 0; y < yCount; ++y)
        {
            float xPosition = std::rand() % _width + 1;
            float yPosition = std::rand() % _height + 1;
            _mediumStarPositions.push_back(juce::Point<float>(xPosition, yPosition));
        }
    }
}

void Stars::computeLargeStars()
{
    int density = 128;
    auto xCount = _width / density;
    auto yCount = _height / density;
    
    _largeStarPositions.resize(static_cast<std::size_t>(xCount * yCount));
    
    for (int x = 0; x < xCount; ++x)
    {
        for (int y = 0; y < yCount; ++y)
        {
            float xPosition = std::rand() % _width + 1;
            float yPosition = std::rand() % _height + 1;
            _largeStarPositions.push_back(juce::Point<float>(xPosition, yPosition));
        }
    }
}

void Stars::paint(juce::Graphics& g)
{
    g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());
    
    juce::Path smallStars;
    for (const auto& position : _smallStarPositions) {
        smallStars.addEllipse(position.x, position.y, 0.4f, 0.4f);
    }
    g.fillPath(smallStars);
    
    juce::Path mediumStars;
    for (const auto& position : _mediumStarPositions) {
        mediumStars.addEllipse(position.x, position.y, 0.7f, 0.7f);
    }
    g.fillPath(mediumStars);
    
    juce::Path largeStars;
    for (const auto& position : _largeStarPositions) {
        largeStars.addEllipse(position.x, position.y, 1.0f, 1.0f);
    }

    g.fillPath(largeStars);
}

void Stars::resized()
{
    _width = getWidth();
    _height = getHeight();

    setBounds(getLocalBounds());
}

}
