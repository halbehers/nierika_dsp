#pragma once

#include <vector>

#include "../NierikaDSPBinaryData.h"

namespace nierika::gui
{

struct AnimatedIcon
{
    std::vector<const char*> frames;
    float durationInSeconds;
};

class AnimatedIcons
{
public:
    static const AnimatedIcon getCapture() { return { { NierikaDSPBinaryData::CaptureFrame0_svg, NierikaDSPBinaryData::CaptureFrame1_svg, NierikaDSPBinaryData::CaptureFrame2_svg, NierikaDSPBinaryData::CaptureFrame3_svg, NierikaDSPBinaryData::CaptureFrame4_svg }, 1.500426f }; }
};

}
