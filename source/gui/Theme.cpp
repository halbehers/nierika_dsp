#include "../../includes/gui/Theme.h"

namespace ui
{

Theme::Color::Color(ThemeColor color):
    _color(color)
{
}

Theme::Color::~Color()
{
}

juce::uint32 Theme::Color::asHex()
{
    return _themeColorToHex[_color];
}

juce::Colour Theme::Color::asJuce()
{
    return juce::Colour(asHex());
}


std::string Theme::Color::asHexString()
{
    std::ostringstream oss;
    oss << "#" << std::uppercase << std::hex << std::setw(6) << std::setfill('0') << (asHex() & 0xFFFFFF);
    return oss.str();
}

Theme::Color Theme::getColor(ThemeColor color)
{
    return Color(color);
}

}
