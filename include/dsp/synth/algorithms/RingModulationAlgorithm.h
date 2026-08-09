#pragma once

#include "../SynthAlgorithm.h"

namespace nierika::dsp
{

// out = lerp(osc1 + osc2, osc1 * osc2, amount01), per channel - a graduated crossfade between
// plain Add (amount01 == 0) and full ring modulation (amount01 == 1), so dialing it in from 0% is
// musical rather than an all-or-nothing switch.
class RingModulationAlgorithm final : public SynthAlgorithm
{
public:
    void setBlockParameters(float amount01) noexcept override;
    [[nodiscard]] StereoSample getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept override;

private:
    float _amount01 = 0.0f;
};

}
