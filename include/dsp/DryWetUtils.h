#pragma once

namespace nierika::dsp
{

class DryWetConverter
{
public:
    template<typename T>
    static std::pair<T, T> splitDryWetValue(T value, juce::dsp::DryWetMixingRule mixingRule = juce::dsp::DryWetMixingRule::balanced)
    {
        jassert(value >= 0.f && value <= 1.f);

        const T mix = juce::jlimit(static_cast<T>(0.0), static_cast<T>(1.0), value);

        T dryValue, wetValue;

        switch (mixingRule)
        {
            case juce::dsp::DryWetMixingRule::balanced:
                dryValue = 2.f * juce::jmin(static_cast<T>(0.5), static_cast<T>(1.0) - mix);
                wetValue = static_cast<T>(2.0) * juce::jmin(static_cast<T>(0.5), mix);
                break;

            case juce::dsp::DryWetMixingRule::linear:
                dryValue = static_cast<T>(1.0) - mix;
                wetValue = mix;
                break;

            case juce::dsp::DryWetMixingRule::sin3dB:
                dryValue = static_cast<T>(std::sin(0.5 * juce::MathConstants<double>::pi * (1.0 - mix)));
                wetValue = static_cast<T>(std::sin(0.5 * juce::MathConstants<double>::pi * mix));
                break;

            case juce::dsp::DryWetMixingRule::sin4p5dB:
                dryValue = static_cast<T>(std::pow(std::sin (0.5 * juce::MathConstants<double>::pi * (1.0 - mix)), 1.5));
                wetValue = static_cast<T>(std::pow(std::sin (0.5 * juce::MathConstants<double>::pi * mix), 1.5));
                break;

            case juce::dsp::DryWetMixingRule::sin6dB:
                dryValue = static_cast<T>(std::pow(std::sin (0.5 * juce::MathConstants<double>::pi * (1.0 - mix)), 2.0));
                wetValue = static_cast<T>(std::pow(std::sin (0.5 * juce::MathConstants<double>::pi * mix), 2.0));
                break;

            case juce::dsp::DryWetMixingRule::squareRoot3dB:
                dryValue = std::sqrt(static_cast<T> (1.0) - mix);
                wetValue = std::sqrt(mix);
                break;

            case juce::dsp::DryWetMixingRule::squareRoot4p5dB:
                dryValue = static_cast<T>(std::pow(std::sqrt(1.0 - mix), 1.5));
                wetValue = static_cast<T>(std::pow(std::sqrt(mix), 1.5));
                break;

            default:
                dryValue = jmin(static_cast<T>(0.5), static_cast<T>(1.0) - mix);
                wetValue = jmin(static_cast<T>(0.5), mix);
                break;
        }

        return std::make_pair<T, T>(dryValue, wetValue);
    }
};

}