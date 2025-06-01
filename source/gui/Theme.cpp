#include <sstream>
#include <ios>
#include <iomanip>
#include <string>

#include "../../include/gui/Theme.h"
#include "../../include/gui/EmbeddedFonts.h"

namespace nierika::gui
{

std::unordered_map<Theme::FontSize, float> Theme::fontSizesToPixels = {
    {
        { TITLE, 32.f },
        { SUBTITLE, 24.f },
        { HEADING, 16.f },
        { CAPTION, 12.f },
        { PARAGRAPH, 11.f },
        { LABEL, 10.f },
        { SMALL, 8.f }
    }
};

std::unordered_map<Theme::FontWeight, juce::Font> Theme::fontWeightToFont = {
    {
        { BOLD, EmbeddedFonts::getBold() },
        { MEDIUM, EmbeddedFonts::getMedium() },
        { REGULAR, EmbeddedFonts::getRegular() },
        { LIGHT, EmbeddedFonts::getLight() },
        { THIN, EmbeddedFonts::getThin() }
    }
};

Theme::Color::Color(const ThemeColor color):
    _color(color)
{
}

juce::uint32 Theme::Color::asHex() const
{
    return _themeColorToHex.at(_color);
}

juce::Colour Theme::Color::asJuce() const
{
    return juce::Colour(asHex());
}


std::string Theme::Color::asHexString() const
{
    std::ostringstream oss;
    oss << "#" << std::uppercase << std::hex << std::setw(6) << std::setfill('0') << (asHex() & 0xFFFFFF);
    return oss.str();
}

Theme::Color Theme::newColor(ThemeColor color)
{
    return Color(color);
}

juce::Font Theme::newFont(FontWeight style, FontSize size)
{
    return fontWeightToFont.at(style).withHeight(getFontSizeInPixels(size));
}

float Theme::getFontSizeInPixels(const FontSize size)
{
    return fontSizesToPixels.at(size);
}

}
