#pragma once

#include "Filter.h"

namespace nierika::dsp
{

class HighPassFilter : public Filter
{
public:
    HighPassFilter() = default;
    explicit HighPassFilter(float defaultCutoffFrenquency);
    ~HighPassFilter() override;
};

}
