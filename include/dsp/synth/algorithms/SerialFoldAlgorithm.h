#pragma once

#include "../SynthAlgorithm.h"

namespace nierika::dsp
{

// out = foldSample(osc1 * (1 + osc2 * amount01 * kSerialFoldMaxGain)), per channel - a genuine
// serial chain (osc2's instantaneous output dynamically drives extra wavefolding applied to osc1)
// rather than a parallel mix.
class SerialFoldAlgorithm final : public SynthAlgorithm
{
public:
    void setBlockParameters(float amount01) noexcept override;
    [[nodiscard]] StereoSample getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept override;

private:
    float _amount01 = 0.0f;
};

}
