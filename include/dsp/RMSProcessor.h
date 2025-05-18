#pragma once

#include "Channel.h"

namespace nierika::dsp
{

class RMSProcessor
{
public:
    RMSProcessor();
    ~RMSProcessor();

    void prepare(double sampleRate, double rampLengthInSeconds);
    void process(const juce::AudioBuffer<float>& buffer);
    [[nodiscard]] float getRMSLevel(Channel channel) const;

private:
    juce::LinearSmoothedValue<float> _rmsLevelLeft, _rmsLevelRight = -60.f;
};

}
