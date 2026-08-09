#include "../../../../include/dsp/synth/algorithms/AddAlgorithm.h"

namespace nierika::dsp
{

StereoSample AddAlgorithm::getNextSample(IPhaseOscillator& oscillator1, IPhaseOscillator& oscillator2) noexcept
{
    const auto osc1Sample = oscillator1.getNextSample(0.0f);
    const auto osc2Sample = oscillator2.getNextSample(0.0f);

    return { osc1Sample.left + osc2Sample.left, osc1Sample.right + osc2Sample.right };
}

}
