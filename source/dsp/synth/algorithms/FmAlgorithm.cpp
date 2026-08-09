#include "../../../../include/dsp/synth/algorithms/FmAlgorithm.h"

namespace nierika::dsp
{

namespace
{
    constexpr float kFmMaxModulationIndex = 8.0f; // tune-by-ear modulation depth (radians) at amount01 == 1
}

void FmAlgorithm::setBlockParameters(float amount01) noexcept
{
    _amount01 = amount01;
}

StereoSample FmAlgorithm::getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept
{
    const auto modulatorSample = oscillator2.getNextSample(0.0f);
    const auto modulatorScalar = (modulatorSample.left + modulatorSample.right) * 0.5f;
    const auto modulationRadians = modulatorScalar * _amount01 * kFmMaxModulationIndex;

    return oscillator1.getNextSample(modulationRadians);
}

}
