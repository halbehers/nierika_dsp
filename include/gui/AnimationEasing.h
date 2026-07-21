#pragma once

#include <cstddef>

#include "AnimatedIcons.h"

// Reusable easing/frame-blend math extracted out of AnimatedSVG so a future
// AnimatedSVGButton (or similar) can reuse it without duplicating the logic.
// The AnimationEasing enum itself lives in AnimatedIcons.h, not here.
namespace nierika::gui::animation
{

// Maps a linear progress value (clamped to [0,1]) to an eased progress value in [0,1].
[[nodiscard]] float applyEasing(AnimationEasing easing, float progress);

struct AnimationFrameBlend
{
    std::size_t frameIndex = 0;
    std::size_t nextFrameIndex = 0;
    float blendAlpha = 0.f;
};

// Given how much time has elapsed in a looping animation, figures out which two frames
// to blend between and by how much. frameRateHz is how many source keyframes the
// animation advances through per second (i.e. 1/frameRateHz seconds per frame).
[[nodiscard]] AnimationFrameBlend computeAnimationFrameBlend(float elapsedSeconds, std::size_t frameCount,
                                                              float frameRateHz, AnimationEasing easing);

}
