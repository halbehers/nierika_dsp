#pragma once

#include <cstddef>

#include "AnimatedIcons.h"

namespace nierika::gui::animation
{

[[nodiscard]] float applyEasing(AnimationEasing easing, float progress);

struct AnimationFrameBlend
{
    std::size_t frameIndex = 0;
    std::size_t nextFrameIndex = 0;
    float blendAlpha = 0.f;
};

[[nodiscard]] AnimationFrameBlend computeAnimationFrameBlend(float elapsedSeconds, std::size_t frameCount,
                                                              float frameRateHz, AnimationEasing easing);

}
