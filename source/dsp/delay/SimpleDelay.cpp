#include "../../../includes/dsp/delay/SimpleDelay.h"

namespace nierika::dsp
{

SimpleDelay::SimpleDelay()
{
}

SimpleDelay::SimpleDelay(float defaultDelayTimeInMs):
    _delayTimeInMsR(defaultDelayTimeInMs),
    _delayTimeInMsL(defaultDelayTimeInMs)
{
}

SimpleDelay::~SimpleDelay()
{
}

void SimpleDelay::setDelayTimeInMsR(float delayTimeInMsR)
{
    _delayTimeInMsR.setTargetValue(delayTimeInMsR);
}

void SimpleDelay::setDelayTimeInMsL(float delayTimeInMsL)
{
    _delayTimeInMsL.setTargetValue(delayTimeInMsL);
}

void SimpleDelay::setDelayNoteTimingR(dsp::Timing::NoteTiming noteTimingR)
{
    _noteTimingR = noteTimingR;
}


void SimpleDelay::setDelayNoteTimingL(dsp::Timing::NoteTiming noteTimingL)
{
    _noteTimingL = noteTimingL;
}

void SimpleDelay::setSideSyncActivation(bool sideSyncActivated)
{
    _sideSyncActivated = sideSyncActivated;
    if (!_sideSyncActivated) setPingPongActivation(false);
}

void SimpleDelay::setPingPongActivation(bool pingPongActivation)
{
    _pingPongActivated = pingPongActivation;
    if (_pingPongActivated) setSideSyncActivation(true);
}

void SimpleDelay::setNoteSyncActivation(bool noteSyncActivated)
{
    _noteSyncActivated = noteSyncActivated;
}

void SimpleDelay::setFeedback(float feedback)
{
    _feedback = feedback;
}

void SimpleDelay::setEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;
}

void SimpleDelay::prepare(const juce::dsp::ProcessSpec& spec)
{
    if (!_isEnabled) return;

    _sampleRate = spec.sampleRate;
    
    _nbOfInputChannels = spec.numChannels;
    const int delayBufferSize = 2 * _sampleRate; // 2s
    
    _delayBuffer.setSize(_nbOfInputChannels, delayBufferSize);

}

void SimpleDelay::process(juce::AudioBuffer<float> &buffer, juce::Optional<double> bpm)
{
    if (bpm.hasValue())
        _bpm = bpm;
    if (!_isEnabled) return;

    const int bufferLength = buffer.getNumSamples();
    const int delayBufferLength = _delayBuffer.getNumSamples();
    
    _delayTimeInMsL.skip(buffer.getNumSamples());
    _delayTimeInMsR.skip(buffer.getNumSamples());
    
    for (int channel = 0; channel < _nbOfInputChannels; ++channel)
    {
        const float* bufferData = buffer.getReadPointer(channel);
        const float* delayBufferData = _delayBuffer.getReadPointer(channel);
        float* dryBuffer = buffer.getWritePointer(channel);
        
        fillDelayBuffer(channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
        getFromDelayBuffer(buffer, channel, bufferLength, delayBufferLength, bufferData, delayBufferData);
        feedbackDelay(channel, bufferLength, delayBufferLength, dryBuffer);
    }

    _writePosition += bufferLength;
    _writePosition %= delayBufferLength;
}

void SimpleDelay::fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    if (delayBufferLength > bufferLength + _writePosition)
    {
        _delayBuffer.copyFromWithRamp(channel, _writePosition, bufferData, bufferLength, _feedback, _feedback);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - _writePosition;

        _delayBuffer.copyFromWithRamp(channel, _writePosition, bufferData, bufferRemaining, _feedback, _feedback);
        const int sampsLeft = bufferLength - bufferRemaining;
        if (sampsLeft > 0)
        {
            _delayBuffer.copyFromWithRamp(channel, 0, bufferData, sampsLeft, _feedback, _feedback);
        }
    }
}

void SimpleDelay::getFromDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData)
{
    const int readPosition = static_cast<int>(delayBufferLength + _writePosition - (_sampleRate * computeTimeInMs(channel) / 1000)) % delayBufferLength;
    
    if (delayBufferLength > bufferLength + readPosition)
    {
        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferLength);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - readPosition;

        buffer.copyFrom(channel, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.copyFrom(channel, bufferRemaining, delayBufferData + readPosition, bufferLength - bufferRemaining);
    }
}

void SimpleDelay::feedbackDelay(int channel, const int bufferLength, const int delayBufferLength, float* dryBuyffer)
{
    if (delayBufferLength > bufferLength + _writePosition)
    {
        _delayBuffer.addFromWithRamp(channel, _writePosition, dryBuyffer, bufferLength, _feedback, _feedback);
    }
    else
    {
        const int bufferRemaining = delayBufferLength - _writePosition;

        _delayBuffer.addFromWithRamp(channel, bufferRemaining, dryBuyffer, bufferRemaining, _feedback, _feedback);
        _delayBuffer.addFromWithRamp(channel, 0, dryBuyffer, bufferLength - bufferRemaining, _feedback, _feedback);
    }
}

float SimpleDelay::computeTimeInMsR()
{
    if (!_noteSyncActivated || !_bpm.hasValue()) return _delayTimeInMsR.getCurrentValue();
    
    return dsp::Timing::getTimeInMs(*_bpm, _noteTimingR);
}

float SimpleDelay::computeTimeInMsL()
{
    if (_pingPongActivated) return computeTimeInMsR() / 2.f;
    if (!_noteSyncActivated || !_bpm.hasValue()) return _delayTimeInMsL.getCurrentValue();
    
    return dsp::Timing::getTimeInMs(*_bpm, _noteTimingL);
}

float SimpleDelay::computeTimeInMs(const int channel)
{
    if (channel == 0) return computeTimeInMsL();

    return _sideSyncActivated ? computeTimeInMsL() : computeTimeInMsR();
}

template<typename T>
T SimpleDelay::msToSamples(T sampleRate, T lengthInMs) noexcept
{
    return sampleRate * lengthInMs * static_cast<T>(.001);
}

}
