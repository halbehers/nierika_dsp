#pragma once

#include "Channel.h"

namespace nierika::dsp
{

class RMSProcessor
{
public:
    RMSProcessor();
    ~RMSProcessor();

    void prepare(const double sampleRate, const double rampLengthInSeconds);
    void process(const juce::AudioBuffer<float>& buffer);
    float getRMSLevel(const Channel channel) const;

private:
    juce::LinearSmoothedValue<float> _rmsLevelLeft, _rmsLevelRight = -60.f;
};

}
