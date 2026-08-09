#include "../../../../include/dsp/synth/algorithms/AmplitudeModulationAlgorithm.h"

namespace nierika::dsp
{

namespace
{
    constexpr float kAmMaxDepth = 1.0f; // tune-by-ear modulation depth at amount01 == 1
}

void AmplitudeModulationAlgorithm::setBlockParameters(float amount01) noexcept
{
    _amount01 = amount01;
}

StereoSample AmplitudeModulationAlgorithm::getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept
{
    const auto osc1Sample = oscillator1.getNextSample(0.0f);
    const auto osc2Sample = oscillator2.getNextSample(0.0f);

    const auto depth = _amount01 * kAmMaxDepth;

    return {
        osc1Sample.left * (1.0f + osc2Sample.left * depth),
        osc1Sample.right * (1.0f + osc2Sample.right * depth)
    };
}

}
