#include "../../include/dsp/CircularAudioBuffer.h"

namespace nierika::dsp
{

CircularAudioBuffer::CircularAudioBuffer():
    _buffer(),
    _writeHeadBuffer()
{
}

CircularAudioBuffer::~CircularAudioBuffer()
{
}

void CircularAudioBuffer::setCapacity(const int capacity)
{
    _capacity = capacity;
}

void CircularAudioBuffer::setTimeInMs(const float timeInMs)
{
    _timeInMs = timeInMs;
    _delayLength = msToSamples(static_cast<float>(_sampleRate), _timeInMs);
}

void CircularAudioBuffer::init()
{
    
}

bool CircularAudioBuffer::isEmpty()
{
    // TODO
    return false;
}

void CircularAudioBuffer::clear()
{
    
}

void CircularAudioBuffer::prepare(double sampleRate, int blockSize)
{
    _sampleRate = sampleRate;
    const auto lengthInSamples = static_cast<int>(msToSamples(static_cast<float>(sampleRate), _timeInMs));
    for (auto& channel : _buffer)
        channel.resize(static_cast<std::size_t>(lengthInSamples), 0.f);
    _delaySize = lengthInSamples;
    _writeHeadBuffer.resize(static_cast<std::size_t>(blockSize));
    _writeHeadIndex = 0;
}

void CircularAudioBuffer::process(juce::AudioBuffer<float>& buffer)
{
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        _writeHeadIndex = (_writeHeadIndex + 1) % _delaySize;
        _writeHeadBuffer[static_cast<std::size_t>(sample)] = _writeHeadIndex;
    }

    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto channelSamples = buffer.getWritePointer(channel);
        auto channelBuffer = _buffer[static_cast<std::size_t>(channel)].data();
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            const auto w = _writeHeadBuffer[static_cast<std::size_t>(sample)];
            channelBuffer[w] = channelSamples[static_cast<std::size_t>(sample)];
            auto r = static_cast<float>(w) - _delayLength;
            if (r < 0.f) r+= _delaySize;
            channelSamples[sample] = channelBuffer[static_cast<int>(r)];
        }
    }
}

template<typename T>
T CircularAudioBuffer::msToSamples(T sampleRate, T timeInMs) noexcept
{
    return sampleRate * timeInMs * static_cast<T>(.001);
}

}
