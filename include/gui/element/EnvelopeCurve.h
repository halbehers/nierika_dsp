#pragma once

#include <array>
#include <memory>
#include <string>

#include "../Component.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

// A draggable 6-stage Delay-Attack-Hold-Decay-Sustain-Release envelope display, APVTS-attached
// directly to the six stage parameters (each drawn as a literal straight-line segment - this is
// only an accurate picture of the underlying DSP if that DSP is itself linear-per-segment, e.g.
// audio::DahdsrEnvelope in the chords_theory plugin). Five draggable breakpoints, one per stage
// boundary after Delay: Attack-end (horizontal drag -> attack time), Hold-end (horizontal ->
// hold time), Decay-end/Sustain (horizontal -> decay time, vertical -> sustain level - the only
// 2-axis point), Release-end (horizontal -> release time). The Delay-end point is also draggable
// (horizontal -> delay time). Time axis is a fixed window (see kVisibleWindowMs in the .cpp) -
// values beyond it are still fully settable via the bound knobs, just visually clamped to the
// widget's right edge while dragging.
class EnvelopeCurve: public Component
{
public:
    // stageParameterIDs, in order: delay, attack, hold, decay, sustain, release.
    EnvelopeCurve(dsp::ParameterManager& parameterManager, const std::string& identifier,
                  const std::array<std::string, 6>& stageParameterIDs);
    ~EnvelopeCurve() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

private:
    enum StageIndex
    {
        Delay = 0,
        Attack = 1,
        Hold = 2,
        Decay = 3,
        Sustain = 4,
        Release = 5,
        kNumStages = 6
    };

    struct StageBinding
    {
        juce::RangedAudioParameter* parameter = nullptr;
        std::unique_ptr<juce::ParameterAttachment> attachment;
        float currentValue = 0.0f;
    };

    // A breakpoint the user can drag - always adjusts one time-stage horizontally; the
    // Decay/Sustain breakpoint also adjusts the Sustain stage vertically (hasLevelDrag).
    struct Breakpoint
    {
        StageIndex timeStage;
        bool hasLevelDrag = false;
    };

    [[nodiscard]] float msToX(float cumulativeMs) const noexcept;
    [[nodiscard]] float xDeltaToMsDelta(float xDelta) const noexcept;
    [[nodiscard]] float levelToY(float level01) const noexcept;
    [[nodiscard]] float yDeltaToLevelDelta(float yDelta) const noexcept;

    [[nodiscard]] std::array<juce::Point<float>, 5> computeBreakpointPositions() const;
    [[nodiscard]] int hitTestBreakpoint(juce::Point<float> position) const;

    std::array<StageBinding, kNumStages> _stages;
    static const std::array<Breakpoint, 5> kBreakpoints;

    int _draggedBreakpoint = -1;
    juce::Point<float> _dragStartMouse;
    float _dragStartTimeMs = 0.0f;
    float _dragStartSustainPercent = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeCurve)
};

}
