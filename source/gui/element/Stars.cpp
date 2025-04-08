#include "../../../includes/gui/element/Stars.h"
#include "../../../includes/gui/Theme.h"

namespace ui::element
{

Stars::Stars(int maxWidth, int maxHeight):
    _width(maxWidth),
    _height(maxHeight)
{
    auto whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    _glowEffect.setGlowProperties(6.0, whiteColor.withAlpha(0.3f));
    
    computeSmallStars();
    computeMediumStars();
    computeLargeStars();
}

Stars::~Stars()
{
}

void Stars::computeSmallStars()
{
    int density = 48;
    auto xCount = _width / density;
    auto yCount = _height / density;
    
    _smallStarPositions.resize(xCount * yCount);
    
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
    int density = 96;
    auto xCount = _width / density;
    auto yCount = _height / density;
    
    _mediumStarPositions.resize(xCount * yCount);
    
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
    
    _largeStarPositions.resize(xCount * yCount);
    
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
    g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());
    
    juce::Path smallStars;
    for (auto& position : _smallStarPositions) {
        smallStars.addEllipse(position.x, position.y, 0.4, 0.4);
    }
    g.fillPath(smallStars);
    
    juce::Path mediumStars;
    for (auto& position : _mediumStarPositions) {
        mediumStars.addEllipse(position.x, position.y, 0.7, 0.7);
    }
    g.fillPath(mediumStars);
    
    juce::Path largeStars;
    for (auto& position : _largeStarPositions) {
        largeStars.addEllipse(position.x, position.y, 1.0, 1.0);
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
