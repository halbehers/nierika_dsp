#include <sstream>
#include <ios>
#include <iomanip>
#include <string>

#include "../../include/gui/Theme.h"

namespace nierika::gui
{

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

Theme::Color Theme::getColor(ThemeColor color)
{
    return Color(color);
}

juce::Font Theme::getFont(FontWeight style, FontSize size) const
{
    return _fontWeightToFont.at(style).withHeight(_fontSizesToPixels.at(size));
}

const float Theme::getFontSizeInPixels(const FontSize size) const
{
    return _fontSizesToPixels.at(size);
}

}
