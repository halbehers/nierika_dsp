#pragma once

#include "../Timing.h"

namespace nierika::dsp
{

class SimpleDelay
{
public:
    SimpleDelay();
    SimpleDelay(float defaultDelayTimeInMs);
    virtual ~SimpleDelay();
    
    void setDelayTimeInMsR(float delayTimeR);
    void setDelayNoteTimingR(dsp::Timing::NoteTiming noteTimingR);
    void setDelayTimeInMsL(float delayTimeL);
    void setDelayNoteTimingL(dsp::Timing::NoteTiming noteTimingL);
    void setSideSyncActivation(bool sideSyncActivated);
    void setNoteSyncActivation(bool noteSyncActivated);
    void setPingPongActivation(bool pingPongActivated);
    void setFeedback(float feedback);

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer, juce::Optional<double> bpm);
    
    void setEnabled(bool isEnabled);
    
protected:
    bool _noteSyncActivated = true;
    juce::LinearSmoothedValue<float> _delayTimeInMsR { 50.0 };
    dsp::Timing::NoteTiming _noteTimingR { dsp::Timing::NoteTiming::NOTE_4 };
    juce::LinearSmoothedValue<float> _delayTimeInMsL { 50.0 };
    dsp::Timing::NoteTiming _noteTimingL { dsp::Timing::NoteTiming::NOTE_4 };
    bool _sideSyncActivated = true;
    bool _pingPongActivated = false;
    
    float _feedback { 0.5 };
    int _sampleRate { 44100 };
    int _nbOfInputChannels;
    bool _isEnabled { true };
    juce::Optional<double> _bpm;
    
    juce::AudioBuffer<float> _delayBuffer;
    int _writePosition { 0 };
    
    void fillDelayBuffer(int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData);
    void getFromDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, const int bufferLength, const int delayBufferLength, const float* bufferData, const float* delayBufferData);
    
    void feedbackDelay(int channel, const int bufferLength, const int delayBufferLength, float* dryBuyffer);
    float computeTimeInMsR();
    float computeTimeInMsL();
    float computeTimeInMs(const int channel);
    template<typename T>
    T msToSamples(T sampleRate, T lengthInMs) noexcept;
};

}
