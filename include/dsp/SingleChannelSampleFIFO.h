#pragma once

namespace nierika::dsp
{

template<typename BlockType>
class SingleChannelSampleFIFO
{
public:
    explicit SingleChannelSampleFIFO(Channel channel):
        _channel(channel)
    {
        _prepared.set(false);
    }
    
    void update(const BlockType& buffer)
    {
        jassert(_prepared.get());
        jassert(buffer.getNumChannels() > _channel);
        
        auto* channelPtr = buffer.getReadPointer(_channel);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            pushNextSample(channelPtr[i]);
        }
    }

    void prepare(int bufferSize)
    {
        _prepared.set(false);

        _size.set(bufferSize);

        _bufferToFill.setSize(1, bufferSize, false, true, true);

        _audioBufferFIFO.prepare(1, bufferSize);
        _fifoIndex = 0;

        _prepared.set(true);
    }

    int getNumCompleteBuffersAvailable() const
    {
        return _audioBufferFIFO.getNumAvailableForReading();
    }

    bool isPrepared() const
    {
        return _prepared.get();
    }

    int getSize() const
    {
        return _size.get();
    }

    bool getAudioBuffer(BlockType& buffer)
    {
        return _audioBufferFIFO.pull(buffer);
    }
    
private:
    Channel _channel;
    int _fifoIndex = 0;
    FIFO<BlockType> _audioBufferFIFO;
    BlockType _bufferToFill;
    juce::Atomic<bool> _prepared = false;
    juce::Atomic<int> _size = 0;
    
    void pushNextSample(float sample)
    {
        if (_fifoIndex == _bufferToFill.getNumSamples())
        {
            auto ok = _audioBufferFIFO.push(_bufferToFill);
            
            juce::ignoreUnused(ok);
            
            _fifoIndex = 0;
        }
        
        _bufferToFill.setSample(0, _fifoIndex, sample);
        ++_fifoIndex;
    }
};

}
