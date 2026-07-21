#include "../../include/gui/AnimationEasing.h"

#include <cmath>

namespace nierika::gui::animation
{

namespace
{
    // Standard WebKit UnitBezier-style cubic-bezier evaluator: solves x(t) = progress for
    // the bezier parameter t via Newton-Raphson (falling back to bisection if the
    // derivative goes flat), then evaluates y(t) - there's no closed-form inverse for a
    // cubic bezier, so this numeric solve is the standard approach browsers use too.
    class UnitBezier
    {
    public:
        UnitBezier(const float x1, const float y1, const float x2, const float y2)
        {
            cx = 3.f * x1;
            bx = 3.f * (x2 - x1) - cx;
            ax = 1.f - cx - bx;

            cy = 3.f * y1;
            by = 3.f * (y2 - y1) - cy;
            ay = 1.f - cy - by;
        }

        [[nodiscard]] float solveForY(const float x) const
        {
            return sampleY(solveForT(x));
        }

    private:
        float ax = 0.f, bx = 0.f, cx = 0.f;
        float ay = 0.f, by = 0.f, cy = 0.f;

        [[nodiscard]] float sampleX(const float t) const { return ((ax * t + bx) * t + cx) * t; }
        [[nodiscard]] float sampleY(const float t) const { return ((ay * t + by) * t + cy) * t; }
        [[nodiscard]] float sampleDX(const float t) const { return (3.f * ax * t + 2.f * bx) * t + cx; }

        [[nodiscard]] float solveForT(const float x) const
        {
            float t = x;
            for (int i = 0; i < 8; ++i)
            {
                const float d = sampleDX(t);
                if (std::abs(d) < 1e-6f) break;

                const float err = sampleX(t) - x;
                if (std::abs(err) < 1e-5f) return t;

                t -= err / d;
            }

            float lo = 0.f, hi = 1.f;
            t = juce::jlimit(0.f, 1.f, x);
            for (int i = 0; i < 30 && hi - lo > 1e-7f; ++i)
            {
                const float err = sampleX(t) - x;
                if (std::abs(err) < 1e-5f) return t;

                if (err > 0.f) hi = t;
                else lo = t;

                t = (lo + hi) * 0.5f;
            }
            return t;
        }
    };
}

float applyEasing(const AnimationEasing easing, const float progress)
{
    const float p = juce::jlimit(0.f, 1.f, progress);

    switch (easing)
    {
        case AnimationEasing::FLAT: return 0.f;
        case AnimationEasing::LINEAR: return p;
        case AnimationEasing::EASE_IN: return UnitBezier(0.42f, 0.f, 1.f, 1.f).solveForY(p);
        case AnimationEasing::EASE_OUT: return UnitBezier(0.f, 0.f, 0.58f, 1.f).solveForY(p);
        case AnimationEasing::EASE_IN_OUT:
        default: return UnitBezier(0.42f, 0.f, 0.58f, 1.f).solveForY(p);
    }
}

AnimationFrameBlend computeAnimationFrameBlend(const float elapsedSeconds, const std::size_t frameCount,
                                                const float frameRateHz, const AnimationEasing easing)
{
    if (frameCount < 2) return {};

    const float safeRateHz = juce::jmax(0.0001f, frameRateHz);
    const float segmentDuration = 1.f / safeRateHz;
    const float totalLoopDuration = static_cast<float>(frameCount) * segmentDuration;

    float loopPosition = std::fmod(elapsedSeconds, totalLoopDuration);
    if (loopPosition < 0.f) loopPosition += totalLoopDuration;

    const float segmentFloat = loopPosition / segmentDuration;
    const auto frameIndex = static_cast<std::size_t>(std::floor(segmentFloat)) % frameCount;
    const float progress = segmentFloat - std::floor(segmentFloat);
    const std::size_t nextFrameIndex = (frameIndex + 1) % frameCount;

    return { frameIndex, nextFrameIndex, applyEasing(easing, progress) };
}

}
