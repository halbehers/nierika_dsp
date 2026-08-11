#pragma once

#include <array>
#include <optional>
#include <string>

#include "../Component.h"
#include "../../dsp/SingleChannelSampleFIFO.h"

namespace nierika::gui::element
{

// A live, circularly-clipped stereo waveform display - modeled directly on SpectrumAnalyzer's own
// plumbing (SingleChannelSampleFIFO fed from the audio thread, a juce::Timer on the message thread
// drains it and rebuilds a juce::Path, paint() only strokes/fills the already-built Path) but
// time-domain instead of frequency-domain, and clipped to a circle instead of drawn as a rectangle.
// Left/right are drawn as two semi-transparent overlaid traces, so a near-mono signal reads as one
// blended trace and genuine stereo content visibly diverges. Audio-thread only for update() calls
// on the FIFOs it's constructed with - everything in this class itself runs on the message thread.
class CircularStereoWaveform : public Component, private juce::Timer
{
public:
    using FIFOType = dsp::SingleChannelSampleFIFO<juce::AudioBuffer<float>>;

    CircularStereoWaveform(const std::string& identifier, FIFOType* leftFifo, FIFOType* rightFifo);
    ~CircularStereoWaveform() override;

    void paint(juce::Graphics&) override;

    // getBackgroundColour() is called fresh inside paint() every repaint, and re-resolves
    // _backgroundThemeColorOverride live rather than caching a resolved Colour - so a live theme
    // change just needs the base class's inherited repaint(), no changeListenerCallback override.
    void setBackgroundColour(juce::Colour colour) { _backgroundThemeColorOverride.reset(); _backgroundOverride = colour; repaint(); }
    void setBackgroundColour(Theme::ThemeColor colorId) { _backgroundThemeColorOverride = colorId; repaint(); }
    void resetBackgroundColour() { _backgroundThemeColorOverride.reset(); _backgroundOverride = juce::Colour(); repaint(); }
    juce::Colour getBackgroundColour() const
    {
        if (_backgroundThemeColorOverride) return Theme::newColor(*_backgroundThemeColorOverride).asJuce();
        return _backgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce());
    }

    void setBorderColour(juce::Colour colour) { _borderOverride = colour; repaint(); }
    void resetBorderColour() { _borderOverride = juce::Colour(); repaint(); }
    juce::Colour getBorderColour() const { return _borderOverride.value_or(Theme::newColor(Theme::ThemeColor::BORDER).asJuce()); }

    void setTraceColour(juce::Colour colour) { _traceColourOverride = colour; }
    void setTraceColour(Theme::ThemeColor colorId) { setTraceColour(Theme::newColor(colorId).asJuce()); }
    [[nodiscard]] juce::Colour getTraceColour() const { return _traceColourOverride.value_or(Theme::newColor(Theme::ThemeColor::ACCENT).asJuce()); }

    void setMagnifyingGlassEnabled(bool enabled) { _magnifyingGlassEnabled = enabled; }
    [[nodiscard]] bool isMagnifyingGlassEnabled() const { return _magnifyingGlassEnabled; }

private:
    // Rolling raw-sample window. Apparent scroll speed is ~sampleRate/kHistorySize (independent of
    // the repaint timer's Hz) - a full block's worth of new samples overwrites a smaller fraction
    // of a bigger window each tick, which is what actually reads as "slower", not a lower repaint
    // rate (that would just make it choppier without changing the underlying scroll speed).
    static constexpr int kHistorySize = 4096;
    static constexpr int kPathResolution = 96;          // decimated point count - reads as "smooth" rather than sample-accurate jagged noise
    static constexpr float kMaxMagnification = 1.0f;    // tune-by-ear peak amplitude boost at dead-centre
    static constexpr float kAmplitudeScale = 0.55f;     // tune-by-ear overall headroom, applied before the lens - keeps even a hot, near-full-scale mix from clipping visually against the circle's edge

    void timerCallback() override;
    void pullIntoHistory(std::array<float, kHistorySize>& history, FIFOType* fifo);
    [[nodiscard]] juce::Path buildWaveformPath(const std::array<float, kHistorySize>& history, juce::Rectangle<float> bounds) const;
    [[nodiscard]] juce::Rectangle<float> getCircleBounds();

    std::array<float, kHistorySize> _leftHistory {};
    std::array<float, kHistorySize> _rightHistory {};
    FIFOType* _leftFifo;
    FIFOType* _rightFifo;
    juce::Path _leftPath;
    juce::Path _rightPath;
    std::optional<juce::Colour> _traceColourOverride = std::nullopt;
    bool _magnifyingGlassEnabled = true; // on by default - it's the intended look, not an opt-in extra
    std::optional<juce::Colour> _backgroundOverride = std::nullopt;
    std::optional<Theme::ThemeColor> _backgroundThemeColorOverride = std::nullopt;
    std::optional<juce::Colour> _borderOverride = std::nullopt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircularStereoWaveform)
};

}
