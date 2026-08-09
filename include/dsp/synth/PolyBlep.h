#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace nierika::dsp
{

// Classic two-sample PolyBLEP correction (Valimaki/Huovilainen) - t is the phase in [0,1) at the
// discontinuity's own local rate, dt is that rate's per-sample phase increment. Clamped to 0.5 so
// a locally inflated dt (e.g. Oscillator's Warp phase-squeeze) degrades gracefully instead of
// producing a mathematically inverted correction region. Shared between Oscillator and
// SubOscillator - both band-limit a Square discontinuity with the identical formula.
inline float polyBlep(double t, double dt) noexcept
{
    const auto clampedDt = juce::jlimit(1.0e-9, 0.5, dt);

    if (t < clampedDt)
    {
        const auto x = t / clampedDt;
        return static_cast<float>(x + x - x * x - 1.0);
    }
    if (t > 1.0 - clampedDt)
    {
        const auto x = (t - 1.0) / clampedDt;
        return static_cast<float>(x * x + x + x + 1.0);
    }
    return 0.0f;
}

}
