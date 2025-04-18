/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace NierikaDSPBinaryData
{
    extern const char*   ArrowDown_svg;
    const int            ArrowDown_svgSize = 230;

    extern const char*   ArrowUp_svg;
    const int            ArrowUp_svgSize = 232;

    extern const char*   Boxes_svg;
    const int            Boxes_svgSize = 947;

    extern const char*   Info_svg;
    const int            Info_svgSize = 744;

    extern const char*   Link_svg;
    const int            Link_svgSize = 2304;

    extern const char*   Minus_svg;
    const int            Minus_svgSize = 267;

    extern const char*   Note_svg;
    const int            Note_svgSize = 732;

    extern const char*   PingPong_svg;
    const int            PingPong_svgSize = 690;

    extern const char*   Plus_svg;
    const int            Plus_svgSize = 404;

    extern const char*   PowerOfficon_svg;
    const int            PowerOfficon_svgSize = 596;

    extern const char*   Brandon_bld_otf;
    const int            Brandon_bld_otfSize = 89928;

    extern const char*   Brandon_light_otf;
    const int            Brandon_light_otfSize = 87228;

    extern const char*   Brandon_med_otf;
    const int            Brandon_med_otfSize = 89536;

    extern const char*   Brandon_reg_otf;
    const int            Brandon_reg_otfSize = 86528;

    extern const char*   Brandon_thin_otf;
    const int            Brandon_thin_otfSize = 90808;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 15;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
