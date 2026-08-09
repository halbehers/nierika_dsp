#include "../../../../include/dsp/synth/algorithms/RingModulationAlgorithm.h"

#include <juce_audio_basics/juce_audio_basics.h>

namespace nierika::dsp
{

void RingModulationAlgorithm::setBlockParameters(float amount01) noexcept
{
    _amount01 = amount01;
}

StereoSample RingModulationAlgorithm::getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept
{
    const auto osc1Sample = oscillator1.getNextSample(0.0f);
    const auto osc2Sample = oscillator2.getNextSample(0.0f);

    const auto addedLeft = osc1Sample.left + osc2Sample.left;
    const auto addedRight = osc1Sample.right + osc2Sample.right;
    const auto productLeft = osc1Sample.left * osc2Sample.left;
    const auto productRight = osc1Sample.right * osc2Sample.right;

    return {
        juce::jmap(_amount01, addedLeft, productLeft),
        juce::jmap(_amount01, addedRight, productRight)
    };
}

}
