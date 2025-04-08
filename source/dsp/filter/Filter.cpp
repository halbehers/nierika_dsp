#include "../../../include/dsp/filter/Filter.h"

namespace nierika::dsp
{
    Filter::Filter(float defaultCutoffFrequency):
        _cutoffFrequency(defaultCutoffFrequency)
    {
    }

    Filter::~Filter()
    {
    }

    void Filter::setCutoffFrequency(float cutoffFrequency)
    {
        _cutoffFrequency = cutoffFrequency;
        resetFilter();
    }

    void Filter::setSampleRate(float sampleRate)
    {
        _sampleRate = sampleRate;
        resetFilter();
    }

    void Filter::setResonance(float resonance)
    {
        _resonance = resonance;
        resetFilter();
    }

    void Filter::setEnabled(bool isEnabled)
    {
        _isEnabled = isEnabled;
    }

    void Filter::resetFilter()
    {
        if (_sampleRate <= 0) return;
        if (_resonance <= 0) return;
        if (_cutoffFrequency <= 0 || _cutoffFrequency > _sampleRate * 0.5) return;

        _stateVariableFilter.state->setCutOffFrequency(_sampleRate, _cutoffFrequency, _resonance);
    }

    void Filter::prepare(const juce::dsp::ProcessSpec& spec)
    {
        if (!_isEnabled) return;

        setSampleRate(spec.sampleRate);

        _stateVariableFilter.reset();
        _stateVariableFilter.prepare(spec);
    }

    void Filter::process(juce::AudioBuffer<float> &buffer)
    {
        if (!_isEnabled) return;

        juce::dsp::AudioBlock<float> block(buffer);
        
        _stateVariableFilter.process(juce::dsp::ProcessContextReplacing<float> (block));
    }
}
