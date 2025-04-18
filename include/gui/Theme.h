#pragma once

#include <unordered_map>
#include <string>

#include "../utils/Singleton.h"

#define COLOR_WHITE 0xFFFFF8F8

#define COLOR_GRAY_50 0xFF808080
#define COLOR_GRAY_100 0xFFAEAEAE
#define COLOR_GRAY_200 0xFF6C6C6C
#define COLOR_GRAY_300 0xFF4A4446
#define COLOR_GRAY_400 0xFF2C2C2C

#define COLOR_BLUE_500 0xFF283F51
#define COLOR_BLUE_400 0xFF334E63
#define COLOR_BLUE_300 0xFF3A607E

#define COLOR_BLACK COLOR_GRAY_400

#define COLOR_TURQUOISE_200 0xFF53F4FB
#define COLOR_TURQUOISE_300 0xFF39B0C1

#define COLOR_RED_100 0xFFF96666
#define COLOR_ORANGE_100 0xFFF2A048

#define COLOR_PRIMARY COLOR_BLUE_400
#define COLOR_ACCENT COLOR_TURQUOISE_200
#define COLOR_TEXT COLOR_WHITE
#define COLOR_DISABLED COLOR_GRAY_100
#define COLOR_DANGER COLOR_RED_100
#define COLOR_WARNING COLOR_ORANGE_100

#define COLOR_TRANSPARENT 0x00FFFFFF
#define COLOR_EMPTY_SHADE COLOR_WHITE
#define COLOR_LIGHTER_SHADE COLOR_GRAY_50
#define COLOR_LIGHT_SHADE COLOR_GRAY_100
#define COLOR_MEDIUM_SHADE COLOR_GRAY_200
#define COLOR_DARK_SHADE COLOR_GRAY_300
#define COLOR_FULL_SHADE COLOR_GRAY_400

namespace nierika::gui
{

class Theme: public utils::Singleton<Theme>
{
public:    
    enum ThemeColor
    {
        TRANSPARENT,
        EMPTY_SHADE,
        LIGHTER_SHADE,
        LIGHT_SHADE,
        MEDIUM_SHADE,
        DARK_SHADE,
        FULL_SHADE,
        PRIMARY,
        ACCENT,
        TEXT,
        DISABLED,
        DANGER,
        WARNING
    };
    
    class Color
    {
    public:
        Color(ThemeColor color);
        ~Color();

        juce::uint32 asHex() const;
        juce::Colour asJuce() const;
        std::string asHexString() const;
        
    private:
        ThemeColor _color;
        std::unordered_map<ThemeColor, juce::uint32> _themeColorToHex {
            {
                { TRANSPARENT, COLOR_TRANSPARENT },
                { EMPTY_SHADE, COLOR_EMPTY_SHADE },
                { LIGHT_SHADE, COLOR_LIGHT_SHADE },
                { LIGHTER_SHADE, COLOR_LIGHTER_SHADE },
                { MEDIUM_SHADE, COLOR_MEDIUM_SHADE },
                { DARK_SHADE, COLOR_DARK_SHADE },
                { FULL_SHADE, COLOR_FULL_SHADE },
                { PRIMARY, COLOR_PRIMARY },
                { ACCENT, COLOR_ACCENT },
                { TEXT, COLOR_TEXT },
                { DISABLED, COLOR_DISABLED },
                { DANGER, COLOR_DANGER },
                { WARNING, COLOR_WARNING }
            }
        };
    };
    
    Color getColor(ThemeColor color);
};

}
