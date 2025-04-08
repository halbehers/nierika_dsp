#pragma once

#include "../ModuleBinaryData.h"

class Icons
{
public:
    static Icons& getInstance()
    {
        static Icons instance;
        return instance;
    }
    Icons(Icons const&) = delete;
    void operator=(Icons const&) = delete;

    const char* getPlus() { return ModuleBinaryData::Plus_svg; }
    const char* getMinus() { return ModuleBinaryData::Minus_svg; }
    const char* getArrowUp() { return ModuleBinaryData::ArrowUp_svg; }
    const char* getArrowDown() { return ModuleBinaryData::ArrowDown_svg; }
    const char* getPowerOff() { return ModuleBinaryData::PowerOfficon_svg; }
    const char* getLink() { return ModuleBinaryData::Link_svg; }
    const char* getPingPong() { return ModuleBinaryData::PingPong_svg; }
    const char* getNote() { return ModuleBinaryData::Note_svg; }
    const char* getBoxes() { return ModuleBinaryData::Boxes_svg; }

private:
    Icons() {}
};
