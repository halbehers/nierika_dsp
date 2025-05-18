#pragma once

#include <array>

namespace nierika::dsp
{

template<typename T, int Capacity = 30>
class FIFO
{
public:
    void prepare(int numChannels, int numSamples)
    {
        static_assert(std::is_same_v<T, juce::AudioBuffer<float>>, "prepare(numChannels, numSamples) should only be used when the FIFO is holding juce::AudioBuffer<float>.");
    
        for (auto& buffer : _buffers)
        {
            buffer.setSize(numChannels, numSamples, false, true, true);
            buffer.clear();
        }
    }

    void prepare(size_t numElements)
    {
        static_assert(std::is_same_v<T, std::vector<float>>, "prepare(numElements) should only be used when the FIFO is holding std::vector<float>.");
    
        for (auto& buffer : _buffers)
        {
            buffer.clear();
            buffer.resize(numElements, 0.f);
            
        }
    }

    bool push(const T& t)
    {
        if (const auto write = _fifo.write(1); write.blockSize1 > 0)
        {
            _buffers[static_cast<std::size_t>(write.startIndex1)] = t;
            return true;
        }

        return false;
    }

    bool pull(T& t)
    {
        if (const auto read = _fifo.read(1); read.blockSize1 > 0)
        {
            t = _buffers[static_cast<std::size_t>(read.startIndex1)];
            return true;
        }
        
        return false;
    }

    int getNumAvailableForReading() const
    {
        return _fifo.getNumReady();
    }
    
private:
    std::array<T, Capacity> _buffers;
    juce::AbstractFifo _fifo { Capacity };
};

}
