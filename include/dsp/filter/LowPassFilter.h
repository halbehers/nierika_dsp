#pragma once

#include "Filter.h"

namespace nierika::dsp
{

class LowPassFilter : public Filter
{
public:
    LowPassFilter() = default;
    explicit LowPassFilter(float defaultCutoffFrenquency);
    ~LowPassFilter() override;
};

}
