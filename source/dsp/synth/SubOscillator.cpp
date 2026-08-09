#include "../../../include/dsp/synth/SubOscillator.h"

#include <cmath>

#include "../../../include/dsp/synth/PolyBlep.h"

namespace nierika::dsp
{

void SubOscillator::setSampleRate(double sampleRate) noexcept
{
    _sampleRate = sampleRate;
}

void SubOscillator::setBlockParameters(const Parameters& parameters) noexcept
{
    _parameters = parameters;
    recomputePhaseIncrement();
}

void SubOscillator::noteOn(int midiNoteNumber) noexcept
{
    _lastMidiNoteNumber = midiNoteNumber;
    recomputePhaseIncrement();
    _phase01 = 0.0;
}

void SubOscillator::recomputePhaseIncrement() noexcept
{
    if (_sampleRate <= 0.0)
        return;

    _baseFrequencyHz = juce::MidiMessage::getMidiNoteInHertz(_lastMidiNoteNumber, static_cast<double>(_parameters.tuningReferenceHz));

    const auto octaveMultiplier = std::pow(2.0, static_cast<double>(_parameters.octave));
    const auto transposeMultiplier = std::pow(2.0, static_cast<double>(_parameters.transposeSemitones) / 12.0);
    const auto frequencyHz = _baseFrequencyHz * octaveMultiplier * transposeMultiplier;

    _phaseIncrement = frequencyHz / _sampleRate;
}

float SubOscillator::getNextSample() noexcept
{
    const auto currentPhase01 = _phase01;
    _phase01 += _phaseIncrement;
    if (_phase01 >= 1.0)
        _phase01 -= std::floor(_phase01);

    // Gated last, after phase has still advanced above - toggling enabled on/off mid-note is
    // click-free by construction, same principle as Oscillator's enabled gating.
    if (!_parameters.enabled)
        return 0.0f;

    const auto sineValue = std::sin(juce::MathConstants<double>::twoPi * currentPhase01);

    auto squareValue = currentPhase01 < 0.5 ? 1.0 : -1.0;
    squareValue += static_cast<double>(polyBlep(currentPhase01, _phaseIncrement));
    squareValue -= static_cast<double>(polyBlep(std::fmod(currentPhase01 + 0.5, 1.0), _phaseIncrement));

    const auto tone = static_cast<double>(juce::jlimit(0.0f, 1.0f, _parameters.tonePercent / 100.0f));
    const auto blended = sineValue * (1.0 - tone) + squareValue * tone;

    const auto outputGain = juce::Decibels::decibelsToGain(_parameters.outputDb, -100.0f);
    return static_cast<float>(blended) * outputGain;
}

}
