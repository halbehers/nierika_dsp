#include "../../../include/gui/element/AnimatedSVG.h"

#include <cmath>
#include <ios>
#include <iomanip>
#include <sstream>

namespace nierika::gui::element
{

namespace
{
    std::string toHexString(const juce::Colour colour)
    {
        std::ostringstream oss;
        oss << "#" << std::uppercase << std::hex << std::setw(6) << std::setfill('0') << (colour.getARGB() & 0xFFFFFF);
        return oss.str();
    }
}

AnimatedSVG::AnimatedSVG(const std::string& identifier, const AnimatedIcon& icon):
    Component(identifier),
    _frames(icon.frames)
{
    jassert(_frames.size() >= 2);

    _frameRateHz = juce::jmax(1, static_cast<int>(std::round(static_cast<float>(_frames.size()) / icon.durationInSeconds)));

    if (_isAnimating)
        startTimerHz(_frameRateHz);
}

void AnimatedSVG::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds();
    const float iconSize = getIconSize();
    const float x = (static_cast<float>(bounds.getWidth()) - iconSize) * 0.5f;
    const float y = (static_cast<float>(bounds.getHeight()) - iconSize) * 0.5f;

    helpers::drawFromAnimatedSVG(g, _frames, static_cast<int>(_currentFrameIndex), toHexString(getColour()),
                                  static_cast<int>(x), static_cast<int>(y),
                                  static_cast<int>(iconSize), static_cast<int>(iconSize),
                                  juce::AffineTransform());
}

void AnimatedSVG::timerCallback()
{
    _currentFrameIndex = (_currentFrameIndex + 1) % _frames.size();
    repaint();
}

void AnimatedSVG::setFrameRate(const int fps)
{
    _frameRateHz = fps;

    if (_isAnimating)
        startTimerHz(_frameRateHz);
}

void AnimatedSVG::setAnimating(const bool shouldAnimate)
{
    _isAnimating = shouldAnimate;

    if (_isAnimating)
        startTimerHz(_frameRateHz);
    else
        stopTimer();
}

}
