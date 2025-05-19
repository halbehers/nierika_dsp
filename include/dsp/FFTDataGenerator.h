#pragma once

#include "FIFO.h"

namespace nierika::dsp
{

enum FFTOrder
{
    order2048 = 11,
    order4096 = 12,
    order8192 = 13
};

template<typename BlockType>
class FFTDataGenerator
{
public:
    void produceFFTDataForRendering(const juce::AudioBuffer<float>& audioData, const float negativeInfinity)
    {
        const auto fftSize = getFFTSize();
        
        _fftData.assign(_fftData.size(), 0);
        auto* readIndex = audioData.getReadPointer(0);
        std::copy(readIndex, readIndex + fftSize, _fftData.begin());
        
        // Apply windowing function to the data.
        _window->multiplyWithWindowingTable(_fftData.data(), static_cast<std::size_t>(fftSize));
        // Render FFt Data.
        _forwardFFT->performFrequencyOnlyForwardTransform(_fftData.data());
        
        int numBins = static_cast<int>(fftSize / 2);
        
        // Normalize FFT values.
        for (int i  = 0; i < numBins; ++i)
        {
            _fftData[static_cast<std::size_t>(i)] /= static_cast<float>(numBins);
        }
        
        // Convert to decibels.
        for (int i  = 0; i < numBins; ++i)
        {
            _fftData[static_cast<std::size_t>(i)] = juce::Decibels::gainToDecibels(static_cast<float>(_fftData[static_cast<std::size_t>(i)]), negativeInfinity);
        }
        
        _fftDataFifo.push(_fftData);
    }
    
    void changeOrder(FFTOrder newOrder)
    {
        _order = newOrder;
        auto fftSize = getFFTSize();
        
        _forwardFFT = std::make_unique<juce::dsp::FFT>(_order);
        _window = std::make_unique<juce::dsp::WindowingFunction<float>>(fftSize, juce::dsp::WindowingFunction<float>::blackmanHarris);
        
        _fftData.clear();
        _fftData.resize(static_cast<std::size_t>(fftSize * 2), 0);
        
        _fftDataFifo.prepare(_fftData.size());
    }
    
    [[nodiscard]] int getFFTSize() const
    {
        return 1 << _order;
    }
    
    [[nodiscard]] int getNumAvailableFFTDataBlocks() const
    {
        return _fftDataFifo.getNumAvailableForReading();
    }
    
    bool getFFTData(BlockType& fftData)
    {
        return _fftDataFifo.pull(fftData);
    }

private:
    FFTOrder _order = order2048;
    BlockType _fftData;
    std::unique_ptr<juce::dsp::FFT> _forwardFFT;
    std::unique_ptr<juce::dsp::WindowingFunction<float>> _window;
    
    FIFO<BlockType> _fftDataFifo;
};

}
