#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../AnimatedIcons.h"
#include "../AnimationEasing.h"
#include "../AnimationFrameInterpolation.h"
#include "../Component.h"
#include "../Helpers.h"

namespace nierika::gui::element
{

class AnimatedSVG: public Component, public juce::Timer
{
public:
    AnimatedSVG(const std::string& identifier, const AnimatedIcon& icon);
    ~AnimatedSVG() override = default;

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    // How many source keyframes the animation advances through per second (not the
    // repaint/Timer rate, which is fixed - see kCrossfadeTimerHz - so crossfading stays
    // smooth regardless of how slow/fast the underlying frame sequence plays).
    void setFrameRate(int fps);
    int getFrameRate() const { return _frameRateHz; }

    void setAnimationEasing(AnimationEasing easing);
    AnimationEasing getAnimationEasing() const { return _animationEasing; }

    void setAnimating(bool shouldAnimate);
    bool isAnimating() const { return _isAnimating; }

    void setColour(juce::Colour colour) { _colourOverride = colour; repaint(); }
    void resetColour() { _colourOverride.reset(); repaint(); }
    juce::Colour getColour() const { return _colourOverride.value_or(Theme::newColor(Theme::ThemeColor::TEXT).asJuce()); }

    void setIconSize(float size) { _iconSize = size; repaint(); }
    void resetIconSize() { _iconSize = -1.f; repaint(); }
    float getIconSize() const { return _iconSize >= 0.f ? _iconSize : static_cast<float>(juce::jmin(getWidth(), getHeight())); }

private:
    static constexpr int kCrossfadeTimerHz = 60;

    std::vector<const char*> _frames;
    float _elapsedSeconds = 0.f;
    animation::AnimationFrameBlend _currentBlend;
    int _frameRateHz = 12;
    AnimationEasing _animationEasing;
    bool _isAnimating = true;
    float _iconSize = -1.f;
    std::optional<juce::Colour> _colourOverride;

    // Per-instance cache of geometrically-interpolated in-between frames, keyed by
    // (frameIndex << 32 | bucket) - nextFrameIndex is always (frameIndex+1) % frameCount
    // for the lifetime of an instance so it's redundant in the key. frameIndex/bucket
    // pairs that turned out structurally incompatible (animation::interpolateSVGFrame
    // returned nullptr) are remembered in _fallbackFramePairs (keyed by frameIndex only,
    // since incompatibility is a property of the frame pair, not of a specific bucket)
    // so the doomed interpolation attempt isn't retried every tick.
    std::unordered_map<std::uint64_t, std::unique_ptr<juce::XmlElement>> _interpolatedFrameCache;
    std::unordered_set<std::size_t> _fallbackFramePairs;

    const juce::XmlElement* getOrCreateInterpolatedFrame(std::size_t frameIndex, int bucket);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnimatedSVG)
};

}
