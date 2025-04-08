#pragma once

#include <vector>

#include "Filter.h"

namespace nierika::dsp
{

class HighPassFilter : public Filter
{
public:
    HighPassFilter();
    HighPassFilter(float defaultCutoffFrenquency);
    ~HighPassFilter() override;
};

}
