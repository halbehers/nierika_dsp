#pragma once

#include "../SynthAlgorithm.h"

namespace nierika::dsp
{

// out = osc1 * (1 + osc2 * amount01 * kAmMaxDepth), per channel - keeps osc1's own fundamental
// recognizable (unlike Ring Modulation, which can suppress it entirely).
class AmplitudeModulationAlgorithm final : public SynthAlgorithm
{
public:
    void setBlockParameters(float amount01) noexcept override;
    [[nodiscard]] StereoSample getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept override;

private:
    float _amount01 = 0.0f;
};

}
