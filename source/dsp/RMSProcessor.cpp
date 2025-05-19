#include "../../include/dsp/RMSProcessor.h"

namespace nierika::dsp
{

void RMSProcessor::prepare(const double sampleRate, const double rampLengthInSeconds)
{
    _rmsLevelLeft.reset(sampleRate, rampLengthInSeconds);
    _rmsLevelRight.reset(sampleRate, rampLengthInSeconds);
    _rmsLevelLeft.setCurrentAndTargetValue(-100.f);
    _rmsLevelRight.setCurrentAndTargetValue(-100.f);
}

void RMSProcessor::process(const juce::AudioBuffer<float>& buffer)
{
    _rmsLevelLeft.skip(buffer.getNumSamples());
    _rmsLevelRight.skip(buffer.getNumSamples());

    if (const auto rmsLevelLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples())); rmsLevelLeft < _rmsLevelLeft.getCurrentValue())
        _rmsLevelLeft.setTargetValue(rmsLevelLeft);
    else _rmsLevelLeft.setCurrentAndTargetValue(rmsLevelLeft);

    if (const auto rmsLevelRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples())); rmsLevelRight < _rmsLevelRight.getCurrentValue())
        _rmsLevelRight.setTargetValue(rmsLevelRight);
    else _rmsLevelRight.setCurrentAndTargetValue(rmsLevelRight);
}

float RMSProcessor::getRMSLevel(const Channel channel) const
{   
    if (channel == Channel::LEFT) return _rmsLevelLeft.getCurrentValue();

    return _rmsLevelRight.getCurrentValue();
}

}