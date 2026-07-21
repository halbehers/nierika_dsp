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
    _frames(icon.frames),
    _animationEasing(icon.easing.value_or(AnimationEasing::LINEAR))
{
    jassert(_frames.size() >= 2);

    _frameRateHz = juce::jmax(1, static_cast<int>(std::round(static_cast<float>(_frames.size()) / icon.durationInSeconds)));

    if (_isAnimating)
        startTimerHz(kCrossfadeTimerHz);
}

void AnimatedSVG::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds();
    const float iconSize = getIconSize();
    const float x = (static_cast<float>(bounds.getWidth()) - iconSize) * 0.5f;
    const float y = (static_cast<float>(bounds.getHeight()) - iconSize) * 0.5f;

    helpers::drawFromAnimatedSVGBlended(g, _frames, static_cast<int>(_currentBlend.frameIndex),
                                         static_cast<int>(_currentBlend.nextFrameIndex), _currentBlend.blendAlpha,
                                         toHexString(getColour()),
                                         static_cast<int>(x), static_cast<int>(y),
                                         static_cast<int>(iconSize), static_cast<int>(iconSize),
                                         juce::AffineTransform());
}

void AnimatedSVG::timerCallback()
{
    _elapsedSeconds += 1.0f / static_cast<float>(kCrossfadeTimerHz);

    const float loopDuration = static_cast<float>(_frames.size()) / static_cast<float>(_frameRateHz);
    _elapsedSeconds = std::fmod(_elapsedSeconds, loopDuration);

    _currentBlend = animation::computeAnimationFrameBlend(_elapsedSeconds, _frames.size(),
                                                            static_cast<float>(_frameRateHz), _animationEasing);
    repaint();
}

void AnimatedSVG::setFrameRate(const int fps)
{
    _frameRateHz = juce::jmax(1, fps);
}

void AnimatedSVG::setAnimationEasing(const AnimationEasing easing)
{
    _animationEasing = easing;
}

void AnimatedSVG::setAnimating(const bool shouldAnimate)
{
    _isAnimating = shouldAnimate;

    if (_isAnimating)
        startTimerHz(kCrossfadeTimerHz);
    else
        stopTimer();
}

}
