#pragma once

#include <vector>

#include "../AnalyzerPathGenerator.h"
#include "../../dsp/SingleChannelSampleFIFO.h"
#include "../../dsp/FFTDataGenerator.h"
#include "../Component.h"

namespace nierika::gui::element
{

class SpectrumAnalyzer: public Component, juce::Timer
{
public:
    using FIFOType = dsp::SingleChannelSampleFIFO<juce::AudioBuffer<float>>;

    SpectrumAnalyzer(const std::string& identifier, juce::AudioProcessor& audioProcessor, FIFOType* leftChannelFifo, FIFOType* rightChannelFifo, bool isMono = false);
    SpectrumAnalyzer(const std::string& identifier, juce::AudioProcessor& audioProcessor, FIFOType* monoChannelFifo);
    SpectrumAnalyzer(juce::AudioProcessor& audioProcessor, FIFOType* leftChannelFifo, FIFOType* rightChannelFifo, bool isMono = false);
    SpectrumAnalyzer(juce::AudioProcessor& audioProcessor, FIFOType* monoChannelFifo);
    ~SpectrumAnalyzer() override;
    
    void paint(juce::Graphics&) override;
    
    void timerCallback() override;
    
    void setMono(bool isMono);
    bool isMono();

private:
    juce::AudioProcessor& _audioProcessor;
    dsp::SingleChannelSampleFIFO<juce::AudioBuffer<float>>* _leftChannelFifo;
    dsp::SingleChannelSampleFIFO<juce::AudioBuffer<float>>* _rightChannelFifo;
    bool _isMono = false;
    
    juce::AudioBuffer<float> _monoBuffer;
    dsp::FFTDataGenerator<std::vector<float>> _leftChannelFFTDataGenerator;
    dsp::FFTDataGenerator<std::vector<float>> _rightChannelFFTDataGenerator;
    AnalyzerPathGenerator<juce::Path> _pathGenerator;
    juce::Path _leftChannelFFTPath;
    juce::Path _rightChannelFFTPath;
    
    void fillFFTDataGenerator(dsp::FFTDataGenerator<std::vector<float>>* fftDataGenerator, dsp::SingleChannelSampleFIFO<juce::AudioBuffer<float>>* fifo);
    void fillPath(juce::Path* path, dsp::FFTDataGenerator<std::vector<float>>* fftDataGenerator);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzer)
};

}
