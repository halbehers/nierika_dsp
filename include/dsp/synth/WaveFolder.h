#pragma once

#include <cmath>

namespace nierika::dsp
{

// Reflects x back into [-1, 1] using a period-4 triangle-wave folding function - the same
// technique Vital/Serum's Fold mode and West-coast/Buchla-style wavefolders use. Any value already
// within [-1, 1] passes through unchanged. Shared between Oscillator and SerialFoldAlgorithm.
inline float foldSample(float x) noexcept
{
    auto wrapped = std::fmod(x + 1.0f, 4.0f);
    if (wrapped < 0.0f)
        wrapped += 4.0f;
    if (wrapped > 2.0f)
        wrapped = 4.0f - wrapped;
    return wrapped - 1.0f;
}

}
