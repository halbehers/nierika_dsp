#pragma once

#include <vector>

#include "Filter.h"

namespace nierika::dsp
{

class LowPassFilter : public Filter
{
public:
    LowPassFilter();
    LowPassFilter(float defaultCutoffFrenquency);
    ~LowPassFilter() override;
};

}
