#pragma once

namespace nierika::dsp
{

class Filter
{
    public:
    Filter();
    Filter(float defaultCutoffFrequency);
    virtual ~Filter();
    
    void setCutoffFrequency(float cutoffFrequency);
    void setSampleRate(float sampleRate);
    void setResonance(float resonance);

    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    
    void setEnabled(bool isEnabled);
    
    protected:
    float _cutoffFrequency = 20.0;
    float _sampleRate;
    float _resonance = 1.0;
    bool _isEnabled = true;
    
    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>, juce::dsp::StateVariableFilter::Parameters<float>> _stateVariableFilter;

    void resetFilter();
};

}
