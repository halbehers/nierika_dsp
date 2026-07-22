#pragma once

#include <optional>
#include <vector>

#include "../NierikaDSPBinaryData.h"

namespace nierika::gui
{

enum class AnimationEasing
{
    FLAT,
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT
};

struct AnimatedIcon
{
    std::vector<const char*> frames;
    float durationInSeconds;
    std::optional<AnimationEasing> easing;
};

class AnimatedIcons
{
public:
    static const AnimatedIcon getCapture() { return { { NierikaDSPBinaryData::CaptureFrame0_svg, NierikaDSPBinaryData::CaptureFrame1_svg, NierikaDSPBinaryData::CaptureFrame2_svg, NierikaDSPBinaryData::CaptureFrame3_svg, NierikaDSPBinaryData::CaptureFrame4_svg }, 1.500426f }; }
};

}
