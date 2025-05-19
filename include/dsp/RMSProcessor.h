#pragma once

namespace nierika::dsp
{

class RMSProcessor
{
public:
    RMSProcessor() = default;
    ~RMSProcessor() = default;

    void prepare(double sampleRate, double rampLengthInSeconds);
    void process(const juce::AudioBuffer<float>& buffer);
    [[nodiscard]] float getRMSLevel(Channel channel) const;

private:
    juce::LinearSmoothedValue<float> _rmsLevelLeft, _rmsLevelRight = -60.f;
};

}
