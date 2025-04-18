#pragma once

#include "../NierikaDSPBinaryData.h"
#include "../utils/Singleton.h"

namespace nierika::gui
{

class Icons: public utils::Singleton<Icons>
{
public:
    const char* getPlus() { return NierikaDSPBinaryData::Plus_svg; }
    const char* getMinus() { return NierikaDSPBinaryData::Minus_svg; }
    const char* getArrowUp() { return NierikaDSPBinaryData::ArrowUp_svg; }
    const char* getArrowDown() { return NierikaDSPBinaryData::ArrowDown_svg; }
    const char* getPowerOff() { return NierikaDSPBinaryData::PowerOfficon_svg; }
    const char* getLink() { return NierikaDSPBinaryData::Link_svg; }
    const char* getPingPong() { return NierikaDSPBinaryData::PingPong_svg; }
    const char* getNote() { return NierikaDSPBinaryData::Note_svg; }
    const char* getBoxes() { return NierikaDSPBinaryData::Boxes_svg; }
    const char* getInfo() { return NierikaDSPBinaryData::Info_svg; }
};

}
