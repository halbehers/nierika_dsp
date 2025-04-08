#include "../../../include/gui/element/SpectrumAnalyzer.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

SpectrumAnalyzer::SpectrumAnalyzer(juce::AudioProcessor& audioProcessor, SpectrumAnalyzer::FIFOType* leftChannelFifo, SpectrumAnalyzer::FIFOType* rightChannelFifo, bool isMono):
    _audioProcessor(audioProcessor),
    _leftChannelFifo(leftChannelFifo),
    _rightChannelFifo(rightChannelFifo),
    _isMono(isMono)
{
    _leftChannelFFTDataGenerator.changeOrder(dsp::FFTOrder::order2048);
    _monoBuffer.setSize(1, _leftChannelFFTDataGenerator.getFFTSize());

    startTimerHz(24);
}

SpectrumAnalyzer::SpectrumAnalyzer(juce::AudioProcessor& audioProcessor, SpectrumAnalyzer::FIFOType* monoChannelFifo):
    _audioProcessor(audioProcessor),
    _leftChannelFifo(monoChannelFifo),
    _rightChannelFifo(nullptr),
    _isMono(true)
{
    startTimerHz(24);
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
    stopTimer();
}

void SpectrumAnalyzer::setMono(bool isMono)
{
    _isMono = isMono;
}

bool SpectrumAnalyzer::isMono()
{
    return _isMono;
}

void SpectrumAnalyzer::timerCallback()
{
    BlockType tempIncomingBuffer;
    
    fillFFTDataGenerator(&_leftChannelFFTDataGenerator, _leftChannelFifo);
    fillPath(&_leftChannelFFTPath, &_leftChannelFFTDataGenerator);
    
    /*if (!_isMono)
    {
        fillFFTDataGenerator(&_rightChannelFFTDataGenerator, _rightChannelFifo);
        fillPath(&_rightChannelFFTPath, &_rightChannelFFTDataGenerator);
    }*/

    repaint();
}

void SpectrumAnalyzer::fillFFTDataGenerator( dsp::FFTDataGenerator<std::vector<float>>* fftDataGenerator, dsp::SingleChannelSampleFIFO<BlockType>* fifo)
{
    BlockType tempIncomingBuffer;

    while (fifo->getNumCompleteBuffersAvailable() > 0)
    {
        if (fifo->getAudioBuffer(tempIncomingBuffer))
        {
            auto size = tempIncomingBuffer.getNumSamples();
            juce::FloatVectorOperations::copy(_monoBuffer.getWritePointer(0, 0),
                                              _monoBuffer.getReadPointer(0, size),
                                              _monoBuffer.getNumSamples() - size);
            juce::FloatVectorOperations::copy(_monoBuffer.getWritePointer(0, _monoBuffer.getNumSamples() - size),
                                              tempIncomingBuffer.getReadPointer(0, 0),
                                              size);
            fftDataGenerator->produceFFTDataForRendering(_monoBuffer, -60.f);
        }
    }
};

void SpectrumAnalyzer::fillPath(juce::Path* path, dsp::FFTDataGenerator<std::vector<float>>* fftDataGenerator)
{
    const auto fftBounds = getLocalBounds().toFloat();
    const auto fftSize = fftDataGenerator->getFFTSize();

    const double binWidth = _audioProcessor.getSampleRate() / static_cast<double>(fftSize);

    while (fftDataGenerator->getNumAvailableFFTDataBlocks() > 0)
    {
        std::vector<float> fftData;
        if (fftDataGenerator->getFFTData(fftData))
        {
            _pathGenerator.generatePath(fftData, fftBounds, fftSize, binWidth, -60.f);
        }
    }

    while (_pathGenerator.getNumPathsAvailable() > 0)
    {
        _pathGenerator.getPath(*path);
    }
}

void SpectrumAnalyzer::paint(juce::Graphics& g)
{
    g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.10f));
    g.strokePath(_leftChannelFFTPath, juce::PathStrokeType(1.f));

    g.setGradientFill(juce::ColourGradient(Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.05f), getWidth() / 2, 0.0, Theme::getInstance().getColor(Theme::ThemeColor::TRANSPARENT).asJuce(), getWidth() / 2, getHeight(), false));
    g.fillPath(_leftChannelFFTPath);
}

}
