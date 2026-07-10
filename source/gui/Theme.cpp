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
        { HEADING, 20.f },
        { CAPTION, 16.f },
        { LABEL, 14.f },
        { PARAGRAPH, 13.f },
        { SMALL, 10.f }
    }
};

Theme::Config Theme::_lastConfig {};

Theme::Palette Theme::buildPreset(const Mode mode)
{
    if (mode == Mode::Light)
    {
        return {
            { TRANSPARENT, juce::Colour(static_cast<juce::uint32>(COLOR_TRANSPARENT)) },
            { EMPTY_SHADE, juce::Colour(0xFF1A1A1A) },
            { LIGHTER_SHADE, juce::Colour(0xFFD8D8D8) },
            { LIGHT_SHADE, juce::Colour(0xFFC4C4C4) },
            { MEDIUM_SHADE, juce::Colour(0xFF9A9A9A) },
            { DARK_SHADE, juce::Colour(0xFF6B6468) },
            { FULL_SHADE, juce::Colour(0xFF232323) },
            { BLACK, juce::Colour(static_cast<juce::uint32>(COLOR_BLACK)) },
            { BACKGROUND, juce::Colour(0xFFF2F2F2) },
            { SECONDARY_BACKGROUND, juce::Colour(0xFFF2F2F2) },
            { BORDER, juce::Colour(0xFFCCCCCC) },
            { PRIMARY, juce::Colour(0xFF3A607E) },
            { ACCENT, juce::Colour(0xFF1AAFBD) },
            { TEXT, juce::Colour(0xFF1A1A1A) },
            { DISABLED, juce::Colour(0xFFB5B5B5) },
            { DANGER, juce::Colour(0xFFD64545) },
            { WARNING, juce::Colour(0xFFC97A1F) }
        };
    }

    // Mode::Dark - byte-for-byte identical to the original hardcoded values.
    return {
        { TRANSPARENT, juce::Colour(static_cast<juce::uint32>(COLOR_TRANSPARENT)) },
        { EMPTY_SHADE, juce::Colour(static_cast<juce::uint32>(COLOR_EMPTY_SHADE)) },
        { LIGHTER_SHADE, juce::Colour(static_cast<juce::uint32>(COLOR_LIGHTER_SHADE)) },
        { LIGHT_SHADE, juce::Colour(static_cast<juce::uint32>(COLOR_LIGHT_SHADE)) },
        { MEDIUM_SHADE, juce::Colour(static_cast<juce::uint32>(COLOR_MEDIUM_SHADE)) },
        { DARK_SHADE, juce::Colour(static_cast<juce::uint32>(COLOR_DARK_SHADE)) },
        { FULL_SHADE, juce::Colour(static_cast<juce::uint32>(COLOR_FULL_SHADE)) },
        { BLACK, juce::Colour(static_cast<juce::uint32>(COLOR_BLACK)) },
        { BACKGROUND, juce::Colour(static_cast<juce::uint32>(COLOR_BACKGROUND)) },
        { SECONDARY_BACKGROUND, juce::Colour(static_cast<juce::uint32>(COLOR_SECONDARY_BACKGROUND)) },
        { BORDER, juce::Colour(static_cast<juce::uint32>(COLOR_BORDER)) },
        { PRIMARY, juce::Colour(static_cast<juce::uint32>(COLOR_PRIMARY)) },
        { ACCENT, juce::Colour(static_cast<juce::uint32>(COLOR_ACCENT)) },
        { TEXT, juce::Colour(static_cast<juce::uint32>(COLOR_TEXT)) },
        { DISABLED, juce::Colour(static_cast<juce::uint32>(COLOR_DISABLED)) },
        { DANGER, juce::Colour(static_cast<juce::uint32>(COLOR_DANGER)) },
        { WARNING, juce::Colour(static_cast<juce::uint32>(COLOR_WARNING)) }
    };
}

Theme::FontSet Theme::buildDefaultFontSet()
{
    return {
        { BOLD, EmbeddedFonts::getBold() },
        { MEDIUM, EmbeddedFonts::getMedium() },
        { REGULAR, EmbeddedFonts::getRegular() },
        { LIGHT, EmbeddedFonts::getLight() },
        { THIN, EmbeddedFonts::getThin() }
    };
}

Theme::Palette Theme::_activePalette = Theme::buildPreset(Theme::Mode::Dark);
Theme::FontSet Theme::_activeFonts = Theme::buildDefaultFontSet();
float Theme::_borderRadius = DEFAULT_BORDER_RADIUS;
juce::ChangeBroadcaster Theme::_changeBroadcaster {};

void Theme::applyConfig(const Config& config)
{
    Palette palette = buildPreset(config.mode);
    if (config.customPalette.has_value())
        for (const auto& [key, value] : *config.customPalette)
            palette[key] = value;
    for (const auto& [key, value] : config.colorOverrides)
        palette[key] = value;
    _activePalette = std::move(palette);

    FontSet fonts = buildDefaultFontSet();
    for (const auto& [weight, font] : config.fontOverrides)
        fonts[weight] = font;
    _activeFonts = std::move(fonts);

    _borderRadius = config.borderRadius.value_or(DEFAULT_BORDER_RADIUS);

    // Theme changes always originate on the message thread (host startup code,
    // or a UI-triggered mode toggle), so synchronous dispatch is safe and keeps
    // listeners' repaint() calls deterministic relative to the change.
    _changeBroadcaster.sendSynchronousChangeMessage();
}

void Theme::configure(const Config& config)
{
    _lastConfig = config;
    applyConfig(_lastConfig);
}

void Theme::setMode(const Mode mode)
{
    _lastConfig.mode = mode;
    applyConfig(_lastConfig);
}

Theme::Mode Theme::getMode()
{
    return _lastConfig.mode;
}

void Theme::setColor(const ThemeColor color, const juce::Colour value)
{
    _lastConfig.colorOverrides[color] = value;
    applyConfig(_lastConfig);
}

void Theme::setPalette(const Palette& palette)
{
    _lastConfig.customPalette = palette;
    applyConfig(_lastConfig);
}

void Theme::setFont(const FontWeight weight, juce::Font font)
{
    _lastConfig.fontOverrides[weight] = std::move(font);
    applyConfig(_lastConfig);
}

void Theme::setFonts(const FontSet& fonts)
{
    for (const auto& [weight, font] : fonts)
        _lastConfig.fontOverrides[weight] = font;
    applyConfig(_lastConfig);
}

void Theme::setBorderRadius(const float radius)
{
    _lastConfig.borderRadius = radius;
    applyConfig(_lastConfig);
}

float Theme::getBorderRadius()
{
    return _borderRadius;
}

void Theme::resetToDefaults()
{
    _lastConfig = Config {};
    applyConfig(_lastConfig);
}

juce::ChangeBroadcaster& Theme::getChangeBroadcaster()
{
    return _changeBroadcaster;
}

Theme::Color::Color(const ThemeColor color):
    _color(color)
{
}

juce::uint32 Theme::Color::asHex() const
{
    return Theme::_activePalette.at(_color).getARGB();
}

juce::Colour Theme::Color::asJuce() const
{
    return Theme::_activePalette.at(_color);
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
    return _activeFonts.at(style).withHeight(getFontSizeInPixels(size));
}

float Theme::getFontSizeInPixels(const FontSize size)
{
    return fontSizesToPixels.at(size);
}

}
