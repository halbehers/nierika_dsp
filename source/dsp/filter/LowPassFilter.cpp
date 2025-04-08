#include "../../../include/dsp/filter/LowPassFilter.h"

namespace dsp
{
    LowPassFilter::LowPassFilter(float defaultCutoffFrenquency):
        Filter::Filter(defaultCutoffFrenquency)
    {
        _stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>
        ::Type::lowPass;
    }

    LowPassFilter::~LowPassFilter()
    {
    }
}
