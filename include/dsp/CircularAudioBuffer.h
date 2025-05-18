#pragma once

#include <array>
#include <vector>

namespace nierika::dsp
{

class CircularAudioBuffer
{
public:
    CircularAudioBuffer();
    ~CircularAudioBuffer();
    
    void setCapacity(int capacity);
    void init();
    void setTimeInMs(float timeInMs);
    
    bool isEmpty();
    void clear();
    
    void prepare(double sampleRate, int blockSize);
    void process(juce::AudioBuffer<float>& buffer);
    
protected:
    int _capacity = 0;
    double _sampleRate = 44100.f;
    float _timeInMs = 14;
    std::array<std::vector<float>, 2> _buffer;
    std::vector<int> _writeHeadBuffer;
    int _writeHeadIndex = 0;
    int _delaySize = 1;
    float _delayLength = 0.f;
    
    template<typename T>
    T msToSamples(T sampleRate, T timeInMs) noexcept;
};

}
