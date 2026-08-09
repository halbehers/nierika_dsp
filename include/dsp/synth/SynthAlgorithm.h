#pragma once

#include <juce_core/juce_core.h>

namespace nierika::dsp
{

struct StereoSample
{
    float left = 0.0f;
    float right = 0.0f;
};

// Minimal per-sample contract a SynthAlgorithm needs from an oscillator - deliberately narrower
// than a full oscillator's own API (unison/warp/fold/etc. stay private to the concrete
// implementation). Implemented by Oscillator; kept as its own interface for plain interface
// segregation, so a combine algorithm only ever sees the one thing it's actually allowed to do:
// pull the next sample, optionally perturbing phase with another oscillator's output first.
class IPhaseOscillator
{
public:
    virtual ~IPhaseOscillator() = default;

    // externalPhaseModulationRadians: added to the oscillator's own free-running phase for exactly
    // one sample before evaluating its shape. 0 for algorithms that don't need it (only FM does).
    [[nodiscard]] virtual StereoSample getNextSample(float externalPhaseModulationRadians = 0.0f) noexcept = 0;
};

// Pluggable strategy for how two oscillators combine into one stereo signal per sample.
class SynthAlgorithm
{
public:
    virtual ~SynthAlgorithm() = default;

    // amount01: this algorithm's own 0..1 parameter (e.g. FM Amount/Ring Mod Mix/AM Depth/Fold
    // Amount). Algorithms without a parameter (Add) ignore it via this default no-op.
    virtual void setBlockParameters(float amount01) noexcept { juce::ignoreUnused(amount01); }

    [[nodiscard]] virtual StereoSample getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept = 0;
};

}
