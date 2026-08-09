#pragma once

#include <array>

#include <juce_audio_basics/juce_audio_basics.h>

#include "SynthAlgorithm.h"

namespace nierika::dsp
{

// A single, reusable per-voice oscillator - a consuming plugin typically owns two independent
// instances, summed (or otherwise combined - see SynthAlgorithm) by whatever it wires up. Per-
// sample signal path, per unison voice: base waveform (PolyBLEP-corrected Sine/Triangle/Saw/
// Square) -> Warp (phase squeeze + zero-fill) -> Shape noise (linear crossfade toward white noise)
// -> Fold (wavefolding), then panned and summed across up to kMaxUnisonVoices detuned voices.
// Implements IPhaseOscillator so a SynthAlgorithm can drive it without depending on this concrete
// type. Audio-thread only: nothing here allocates outside construction.
//
// PolyBLEP band-limits the base waveform's own Saw/Square discontinuities. Warp and Fold are
// nonlinear stages that generate their own extra high-frequency content on top of that - fully
// band-limiting those would need oversampling, which is out of scope for now; this is a
// deliberate, documented limit, not an oversight.
class Oscillator : public IPhaseOscillator
{
public:
    enum class Shape
    {
        Sine,
        Triangle,
        Saw,
        Square
    };

    static constexpr int kMaxUnisonVoices = 16;

    struct Parameters
    {
        bool  enabled = true;              // false silences output entirely, phase still advances
        Shape shape = Shape::Sine;
        float shapeNoisePercent = 0.0f;    // 0..100
        int   octave = 0;                  // -3..+3
        int   transposeSemitones = 0;      // -12..+12, added on top of octave
        float detuneCents = 0.0f;          // -50..+50
        float warpPercent = 0.0f;          // -100..+100
        float foldPercent = 0.0f;          // 0..100
        float outputDb = 0.0f;             // -24..+6
        int   unisonVoices = 1;            // 1..kMaxUnisonVoices - 1 = today's exact single-voice behavior
        float unisonDetuneCents = 0.0f;    // 0..50, symmetric spread across voices
        float unisonStereoPercent = 0.0f;  // 0..100
        float phasePercent = 0.0f;         // 0..100 -> 0..2pi start phase
        bool  phaseRandomizeEnabled = false;
        float tuningReferenceHz = 440.0f;  // concert pitch (frequency of A4) used to convert MIDI note -> Hz
    };

    // Alias, not a distinct type - preserves Oscillator::StereoSample for existing callers while
    // satisfying getNextSample()'s override (which must return exactly IPhaseOscillator's type).
    using StereoSample = nierika::dsp::StereoSample;

    void setSampleRate(double sampleRate) noexcept;

    // Block-rate snapshot, like VoiceFilter/Lfo - re-read once per renderNextBlock, not per sample,
    // so every live knob turn is audible within about one host block on an already-held note.
    void setBlockParameters(const Parameters& parameters) noexcept;

    // Sets the base playing frequency from a MIDI note and resets (or, if phaseRandomizeEnabled,
    // randomizes) every unison voice's starting phase.
    void noteOn(int midiNoteNumber) noexcept;

    [[nodiscard]] StereoSample getNextSample(float externalPhaseModulationRadians = 0.0f) noexcept override;

private:
    struct UnisonVoiceState
    {
        double phase01 = 0.0;
        double phaseIncrement = 0.0;
        juce::Random random;
    };

    [[nodiscard]] float computeShapeValue(double phase01, double phaseIncrement) const noexcept;
    [[nodiscard]] float computeWarpedShapeValue(double outerPhase01, double outerPhaseIncrement) const noexcept;

    // Derives _baseFrequencyHz (from _lastMidiNoteNumber and _parameters.tuningReferenceHz) and
    // every unison phase increment - called from both setBlockParameters() (parameters, including
    // tuningReferenceHz, just changed) and noteOn() (the MIDI note just changed), so either call
    // order is safe and never leaves increments computed from a stale note, tuning, or parameter
    // snapshot. This is what makes tuningReferenceHz (and Octave/Detune/Transpose) live-updatable
    // on an already-held note, not just at the next noteOn().
    void recomputePhaseIncrements() noexcept;

    double _sampleRate = 44100.0;
    int _lastMidiNoteNumber = 60;
    double _baseFrequencyHz = 440.0; // derived; see recomputePhaseIncrements()
    Parameters _parameters;

    std::array<UnisonVoiceState, kMaxUnisonVoices> _unisonVoices;
};

}
