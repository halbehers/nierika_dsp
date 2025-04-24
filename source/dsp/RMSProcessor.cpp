#include "../../include/dsp/RMSProcessor.h"

namespace nierika::dsp
{

RMSProcessor::RMSProcessor()
{
}

RMSProcessor::~RMSProcessor()
{
}

void RMSProcessor::prepare(const double sampleRate, const double rampLengthInSeconds)
{
    _rmsLevelLeft.reset(sampleRate, 0.5);
    _rmsLevelRight.reset(sampleRate, 0.5);
    _rmsLevelLeft.setCurrentAndTargetValue(-100.f);
    _rmsLevelRight.setCurrentAndTargetValue(-100.f);
}

void RMSProcessor::process(const juce::AudioBuffer<float>& buffer)
{
    _rmsLevelLeft.skip(buffer.getNumSamples());
    _rmsLevelRight.skip(buffer.getNumSamples());
    
    const auto rmsLevelLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    if (rmsLevelLeft < _rmsLevelLeft.getCurrentValue())
        _rmsLevelLeft.setTargetValue(rmsLevelLeft);
    else _rmsLevelLeft.setCurrentAndTargetValue(rmsLevelLeft);
    
    const auto rmsLevelRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
    if (rmsLevelRight < _rmsLevelRight.getCurrentValue())
        _rmsLevelRight.setTargetValue(rmsLevelRight);
    else _rmsLevelRight.setCurrentAndTargetValue(rmsLevelRight);
}

float RMSProcessor::getRMSLevel(const Channel channel) const
{   
    if (channel == Channel::LEFT) return _rmsLevelLeft.getCurrentValue();

    return _rmsLevelRight.getCurrentValue();
}

}