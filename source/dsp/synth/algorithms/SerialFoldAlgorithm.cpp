#include "../../../../include/dsp/synth/algorithms/SerialFoldAlgorithm.h"

#include "../../../../include/dsp/synth/WaveFolder.h"

namespace nierika::dsp
{

namespace
{
    constexpr float kSerialFoldMaxGain = 6.0f; // tune-by-ear pre-gain multiplier at amount01 == 1, matches Oscillator's own Fold stage
}

void SerialFoldAlgorithm::setBlockParameters(float amount01) noexcept
{
    _amount01 = amount01;
}

StereoSample SerialFoldAlgorithm::getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept
{
    const auto osc1Sample = oscillator1.getNextSample(0.0f);
    const auto osc2Sample = oscillator2.getNextSample(0.0f);

    const auto depth = _amount01 * kSerialFoldMaxGain;

    return {
        foldSample(osc1Sample.left * (1.0f + osc2Sample.left * depth)),
        foldSample(osc1Sample.right * (1.0f + osc2Sample.right * depth))
    };
}

}
