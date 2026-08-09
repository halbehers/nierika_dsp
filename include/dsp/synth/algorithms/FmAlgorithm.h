#pragma once

#include "../SynthAlgorithm.h"

namespace nierika::dsp
{

// Classic 2-operator FM: oscillator2 (the modulator) is sampled first, its L/R averaged into a
// scalar, scaled by amount01 into a modulation index in radians, and fed into oscillator1's (the
// carrier's) own phase for that sample. Only the carrier's resulting sample is returned - the
// modulator is not separately audible, matching standard 2-op FM (Amount = 0 is silent from the
// modulator's own perspective, but the carrier still plays normally, unmodulated).
class FmAlgorithm final : public SynthAlgorithm
{
public:
    void setBlockParameters(float amount01) noexcept override;
    [[nodiscard]] StereoSample getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept override;

private:
    // Audio-thread only, like Oscillator's own _parameters - setBlockParameters/getNextSample are
    // both always called from the same voice's audio-thread call stack, never cross-thread.
    float _amount01 = 0.0f;
};

}
