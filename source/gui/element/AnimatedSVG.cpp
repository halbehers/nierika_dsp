#include "../../../include/gui/element/AnimatedSVG.h"

#include <cmath>
#include <cstring>
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

    // Seamless-loop exports (e.g. CSS @keyframes authored with matching 0%/100% values)
    // often bake a redundant final frame identical to the first, so the wrap-around
    // segment between the last two frames holds a static image for a full segment
    // duration - drop it so the loop closes directly from the last unique frame.
    while (_frames.size() > 2 && std::strcmp(_frames.back(), _frames.front()) == 0)
        _frames.pop_back();

    _frameRateHz = juce::jmax(1, static_cast<int>(std::round(static_cast<float>(_frames.size()) / icon.durationInSeconds)));

    if (_isAnimating)
        startTimerHz(kCrossfadeTimerHz);
}

void AnimatedSVG::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds();
    const float iconSize = getIconSize();
    const int ix = static_cast<int>((static_cast<float>(bounds.getWidth()) - iconSize) * 0.5f);
    const int iy = static_cast<int>((static_cast<float>(bounds.getHeight()) - iconSize) * 0.5f);
    const int size = static_cast<int>(iconSize);
    const auto colHex = toHexString(getColour());

    const auto frameIndex = _currentBlend.frameIndex;
    const int bucket = static_cast<int>(std::round(_currentBlend.blendAlpha * static_cast<float>(kCrossfadeTimerHz)));

    if (bucket <= 0)
    {
        helpers::drawFromAnimatedSVG(g, _frames, static_cast<int>(frameIndex), colHex, ix, iy, size, size, juce::AffineTransform());
    }
    else if (bucket >= kCrossfadeTimerHz)
    {
        helpers::drawFromAnimatedSVG(g, _frames, static_cast<int>(_currentBlend.nextFrameIndex), colHex, ix, iy, size, size, juce::AffineTransform());
    }
    else if (const auto* interpolated = getOrCreateInterpolatedFrame(frameIndex, bucket))
    {
        helpers::drawFromInterpolatedSVG(g, *interpolated, colHex, ix, iy, size, size, juce::AffineTransform());
    }
    else
    {
        helpers::drawFromAnimatedSVGBlended(g, _frames, static_cast<int>(frameIndex),
                                             static_cast<int>(_currentBlend.nextFrameIndex), _currentBlend.blendAlpha,
                                             colHex, ix, iy, size, size, juce::AffineTransform());
    }
}

const juce::XmlElement* AnimatedSVG::getOrCreateInterpolatedFrame(const std::size_t frameIndex, const int bucket)
{
    if (_fallbackFramePairs.count(frameIndex) > 0)
        return nullptr;

    const auto key = (static_cast<std::uint64_t>(frameIndex) << 32) | static_cast<std::uint32_t>(bucket);
    if (const auto it = _interpolatedFrameCache.find(key); it != _interpolatedFrameCache.end())
        return it->second.get();

    const std::size_t nextFrameIndex = (frameIndex + 1) % _frames.size();
    const float t = static_cast<float>(bucket) / static_cast<float>(kCrossfadeTimerHz);

    auto interpolated = animation::interpolateSVGFrame(_frames[frameIndex], _frames[nextFrameIndex], t);
    if (interpolated == nullptr)
    {
        _fallbackFramePairs.insert(frameIndex);
        return nullptr;
    }

    const auto* raw = interpolated.get();
    _interpolatedFrameCache.emplace(key, std::move(interpolated));
    return raw;
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
