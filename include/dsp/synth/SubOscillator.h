#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace nierika::dsp
{

// A single, dedicated low-end layer - a consuming plugin typically owns exactly one instance
// (unlike Oscillator, which it typically owns two of), summed alongside its main oscillators.
// Deliberately simple compared to Oscillator: no unison, warp, fold, shape noise, or phase
// controls - just a PolyBLEP-corrected sine-to-square blend (Tone), tuned independently via its
// own Octave/Transpose. Mono by design (summed centered into both output channels by the caller).
// Audio-thread only: nothing here allocates outside construction.
class SubOscillator
{
public:
    struct Parameters
    {
        bool  enabled = false;             // false silences output entirely, phase still advances
        int   octave = -1;                 // -3..+3
        int   transposeSemitones = 0;      // -12..+12, added on top of octave
        float tonePercent = 0.0f;          // 0..100, 0 = pure sine, 100 = PolyBLEP square
        float outputDb = 0.0f;             // -24..+6
        float tuningReferenceHz = 440.0f;  // concert pitch (frequency of A4) used to convert MIDI note -> Hz
    };

    void setSampleRate(double sampleRate) noexcept;

    // Block-rate snapshot, like Oscillator/VoiceFilter/Lfo - re-read once per renderNextBlock, not
    // per sample.
    void setBlockParameters(const Parameters& parameters) noexcept;

    // Sets the playing frequency from a MIDI note and resets the starting phase.
    void noteOn(int midiNoteNumber) noexcept;

    [[nodiscard]] float getNextSample() noexcept;

private:
    void recomputePhaseIncrement() noexcept;

    double _sampleRate = 44100.0;
    int _lastMidiNoteNumber = 60;
    double _baseFrequencyHz = 440.0; // derived; see recomputePhaseIncrement()
    Parameters _parameters;

    double _phase01 = 0.0;
    double _phaseIncrement = 0.0;
};

}
