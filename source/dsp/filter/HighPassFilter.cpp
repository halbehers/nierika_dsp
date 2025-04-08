#include "../../../includes/dsp/filter/HighPassFilter.h"

namespace dsp
{
    HighPassFilter::HighPassFilter(float defaultCutoffFrenquency):
        Filter::Filter(defaultCutoffFrenquency)
    {
        _stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>
        ::Type::highPass;
    }

    HighPassFilter::~HighPassFilter()
    {
    }
}
