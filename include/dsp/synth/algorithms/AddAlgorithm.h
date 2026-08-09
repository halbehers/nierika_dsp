#pragma once

#include "../SynthAlgorithm.h"

namespace nierika::dsp
{

// out = osc1 + osc2, per channel - the baseline combine algorithm. No parameters. Byte-for-byte
// what a simple parallel oscillator mix always was before SynthAlgorithm existed.
class AddAlgorithm final : public SynthAlgorithm
{
public:
    [[nodiscard]] StereoSample getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept override;
};

}
