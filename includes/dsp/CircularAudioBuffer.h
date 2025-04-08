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
    
    void setCapacity(const int capacity);
    int getCapacity();
    void init();
    void setTimeInMs(const float timeInMs);
    
    bool isEmpty();
    void clear();
    
    void prepare(double sampleRate, int blockSize);
    void process(juce::AudioBuffer<float>& buffer);
    
protected:
    int _capacity;
    double _sampleRate;
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
