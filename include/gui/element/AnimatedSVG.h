#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../AnimatedIcons.h"
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

    void setFrameRate(int fps);
    int getFrameRate() const { return _frameRateHz; }

    void setAnimating(bool shouldAnimate);
    bool isAnimating() const { return _isAnimating; }

    void setColour(juce::Colour colour) { _colourOverride = colour; repaint(); }
    void resetColour() { _colourOverride.reset(); repaint(); }
    juce::Colour getColour() const { return _colourOverride.value_or(Theme::newColor(Theme::ThemeColor::TEXT).asJuce()); }

    void setIconSize(float size) { _iconSize = size; repaint(); }
    void resetIconSize() { _iconSize = -1.f; repaint(); }
    float getIconSize() const { return _iconSize >= 0.f ? _iconSize : static_cast<float>(juce::jmin(getWidth(), getHeight())); }

private:
    std::vector<const char*> _frames;
    std::size_t _currentFrameIndex = 0;
    int _frameRateHz = 12;
    bool _isAnimating = true;
    float _iconSize = -1.f;
    std::optional<juce::Colour> _colourOverride;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnimatedSVG)
};

}
