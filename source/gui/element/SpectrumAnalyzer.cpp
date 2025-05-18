#include "../../../include/gui/element/SpectrumAnalyzer.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

SpectrumAnalyzer::SpectrumAnalyzer(const std::string& identifier, juce::AudioProcessor& audioProcessor, SpectrumAnalyzer::FIFOType* leftChannelFifo, SpectrumAnalyzer::FIFOType* rightChannelFifo, bool isMono):
    Component(identifier),
    _audioProcessor(audioProcessor),
    _leftChannelFifo(leftChannelFifo),
    _rightChannelFifo(rightChannelFifo),
    _isMono(isMono)
{
    _leftChannelFFTDataGenerator.changeOrder(dsp::FFTOrder::order2048);
    _monoBuffer.setSize(1, _leftChannelFFTDataGenerator.getFFTSize());

    startTimerHz(24);
}

SpectrumAnalyzer::SpectrumAnalyzer(const std::string& identifier, juce::AudioProcessor& audioProcessor, SpectrumAnalyzer::FIFOType* monoChannelFifo):
    Component(identifier),
    _audioProcessor(audioProcessor),
    _leftChannelFifo(monoChannelFifo),
    _rightChannelFifo(nullptr),
    _isMono(true)
{
    startTimerHz(24);
}

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

bool SpectrumAnalyzer::isMono() const
{
    return _isMono;
}

void SpectrumAnalyzer::timerCallback()
{
    juce::AudioBuffer<float> tempIncomingBuffer;
    
    fillFFTDataGenerator(&_leftChannelFFTDataGenerator, _leftChannelFifo);
    fillPath(&_leftChannelFFTPath, &_leftChannelFFTDataGenerator);
    
    /*if (!_isMono)
    {
        fillFFTDataGenerator(&_rightChannelFFTDataGenerator, _rightChannelFifo);
        fillPath(&_rightChannelFFTPath, &_rightChannelFFTDataGenerator);
    }*/

    repaint();
}

void SpectrumAnalyzer::fillFFTDataGenerator( dsp::FFTDataGenerator<std::vector<float>>* fftDataGenerator, dsp::SingleChannelSampleFIFO<juce::AudioBuffer<float>>* fifo)
{
    juce::AudioBuffer<float> tempIncomingBuffer;

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
}

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
            _pathGenerator.generatePath(fftData, fftBounds, fftSize, static_cast<float>(binWidth), -60.f);
        }
    }

    while (_pathGenerator.getNumPathsAvailable() > 0)
    {
        _pathGenerator.getPath(*path);
    }
}

void SpectrumAnalyzer::paint(juce::Graphics& g)
{
    juce::Path filledPath = _leftChannelFFTPath;
    float startX = 0.f;
    float endX = 0.f;

    juce::Path::Iterator it(_leftChannelFFTPath);

    bool firstPointFound = false;
    while (it.next())
    {
        if (!firstPointFound)
        {
            startX = it.x1;
            firstPointFound = true;
        }

        endX = it.x2;
    }

    const auto bottom = static_cast<float>(getHeight());

    filledPath.lineTo(endX, bottom);
    filledPath.lineTo(startX, bottom);
    filledPath.closeSubPath();

    g.setGradientFill(juce::ColourGradient(Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.2f), getWidth() / 2, 0.0, Theme::getInstance().getColor(Theme::ThemeColor::TRANSPARENT).asJuce(), getWidth() / 2, getHeight(), false));
    g.fillPath(filledPath);

    g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.5f));
    g.strokePath(_leftChannelFFTPath, juce::PathStrokeType(1.f));
}

}
