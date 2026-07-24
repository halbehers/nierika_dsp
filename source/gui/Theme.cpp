#include <sstream>
#include <ios>
#include <iomanip>
#include <string>

#include "../../include/gui/Theme.h"
#include "../../include/gui/EmbeddedFonts.h"

namespace nierika::gui
{

std::unordered_map<Theme::FontSize, float> Theme::defaultFontSizesToPixels = {
    {
        { Theme::FontSize::TITLE, 36.f },
        { Theme::FontSize::SUBTITLE, 32.f },
        { Theme::FontSize::HEADING, 30.f },
        { Theme::FontSize::CAPTION, 24.f },
        { Theme::FontSize::LABEL, 22.f },
        { Theme::FontSize::PARAGRAPH, 18.f },
        { Theme::FontSize::SMALL, 16.f }
    }
};

std::unordered_map<Theme::FontSize, float> Theme::monospacedFontSizesToPixels = {
    {
        { Theme::FontSize::TITLE, 30.f },
        { Theme::FontSize::SUBTITLE, 26.f },
        { Theme::FontSize::HEADING, 24.f },
        { Theme::FontSize::CAPTION, 18.f },
        { Theme::FontSize::LABEL, 16.f },
        { Theme::FontSize::PARAGRAPH, 12.f },
        { Theme::FontSize::SMALL, 10.f }
    }
};

Theme::Config Theme::_lastConfig {};

Theme::PaletteSet Theme::buildPreset()
{
    return
    {
        {
            Mode::LIGHT,
            {
                { TRANSPARENT_COLOR, juce::Colour(static_cast<juce::uint32>(COLOR_TRANSPARENT)) },
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
                { INVERTED_TEXT, juce::Colour(0xFF1A1A1A) },
                { DISABLED, juce::Colour(0xFFB5B5B5) },
                { DANGER, juce::Colour(0xFFD64545) },
                { DANGER_BORDER, juce::Colour(0xFFD64545) },
                { WARNING, juce::Colour(0xFFC97A1F) },
                { WARNING_BORDER, juce::Colour(0xFFC97A1F) }
            }
        },
        {
            Mode::DARK,
            {
                { TRANSPARENT_COLOR, juce::Colour(static_cast<juce::uint32>(COLOR_TRANSPARENT)) },
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
                { INVERTED_TEXT, juce::Colour(static_cast<juce::uint32>(COLOR_INVERTED_TEXT)) },
                { DISABLED, juce::Colour(static_cast<juce::uint32>(COLOR_DISABLED)) },
                { DANGER, juce::Colour(static_cast<juce::uint32>(COLOR_DANGER)) },
                { DANGER_BORDER, juce::Colour(static_cast<juce::uint32>(COLOR_DANGER)) },
                { WARNING, juce::Colour(static_cast<juce::uint32>(COLOR_WARNING)) },
                { WARNING_BORDER, juce::Colour(static_cast<juce::uint32>(COLOR_WARNING)) }
            }
        }
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

Theme::FontSet Theme::buildDefaultMonospaceFontSet()
{
    const juce::String monoName = juce::Font::getDefaultMonospacedFontName();
    return {
        { BOLD, EmbeddedFonts::getMonospaceBold() },
        { MEDIUM, EmbeddedFonts::getMonospaceRegular() },
        { REGULAR, EmbeddedFonts::getMonospaceRegular() },
        { LIGHT, EmbeddedFonts::getMonospaceRegular() },
        { THIN, EmbeddedFonts::getMonospaceRegular() }
    };
}

Theme::PaletteSet Theme::_activePaletteSet = Theme::buildPreset();
Theme::FontSet Theme::_activeFonts = Theme::buildDefaultFontSet();
Theme::FontSet Theme::_activeMonospaceFonts = Theme::buildDefaultMonospaceFontSet();
float Theme::_borderRadius = DEFAULT_BORDER_RADIUS;
float Theme::_thinHeight = DEFAULT_THIN_HEIGHT;
float Theme::_largeHeight = DEFAULT_LARGE_HEIGHT;
juce::ChangeBroadcaster Theme::_changeBroadcaster {};

void Theme::applyConfig(const Config& config)
{
    PaletteSet palette = buildPreset();
    if (config.customPaletteSet.has_value())
        for (const auto& [mode, colors] : *config.customPaletteSet)
            for (const auto& [key, value] : colors)
                palette[mode][key] = value;
    for (const auto& [mode, colors] : config.colorOverrides)
        for (const auto& [key, value] : colors)
            palette[mode][key] = value;
    _activePaletteSet = std::move(palette);

    FontSet fonts = buildDefaultFontSet();
    for (const auto& [weight, font] : config.fontOverrides)
        fonts[weight] = font;
    _activeFonts = std::move(fonts);

    FontSet monospaceFonts = buildDefaultMonospaceFontSet();
    for (const auto& [weight, font] : config.monospaceFontOverrides)
        monospaceFonts[weight] = font;
    _activeMonospaceFonts = std::move(monospaceFonts);

    _borderRadius = config.borderRadius.value_or(DEFAULT_BORDER_RADIUS);
    _thinHeight = config.thinHeight.value_or(DEFAULT_THIN_HEIGHT);
    _largeHeight = config.largeHeight.value_or(DEFAULT_LARGE_HEIGHT);

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

void Theme::setColor(const Mode mode, const ThemeColor color, const juce::Colour value)
{
    _lastConfig.colorOverrides[mode][color] = value;
    applyConfig(_lastConfig);
}

void Theme::setPaletteSet(const PaletteSet& paletteSet)
{
    _lastConfig.customPaletteSet = paletteSet;
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

void Theme::setMonospaceFont(const FontWeight weight, juce::Font font)
{
    _lastConfig.monospaceFontOverrides[weight] = std::move(font);
    applyConfig(_lastConfig);
}

void Theme::setMonospaceFonts(const FontSet& fonts)
{
    for (const auto& [weight, font] : fonts)
        _lastConfig.monospaceFontOverrides[weight] = font;
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

void Theme::setThinHeight(const float height)
{
    _lastConfig.thinHeight = height;
    applyConfig(_lastConfig);
}

float Theme::getThinHeight()
{
    return _thinHeight;
}

void Theme::setLargeHeight(const float height)
{
    _lastConfig.largeHeight = height;
    applyConfig(_lastConfig);
}

float Theme::getLargeHeight()
{
    return _largeHeight;
}

float Theme::resolveHeight(const HeightType type, const float autoHeight)
{
    switch (type)
    {
        case HeightType::THIN: return _thinHeight;
        case HeightType::LARGE: return _largeHeight;
        case HeightType::AUTO:
        default: return autoHeight;
    }
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

Theme::Color::Color(const Mode mode, const ThemeColor color):
    _mode(mode),
    _color(color)
{
}

juce::uint32 Theme::Color::asHex() const
{
    return Theme::_activePaletteSet.at(_mode).at(_color).getARGB();
}

juce::Colour Theme::Color::asJuce() const
{
    return Theme::_activePaletteSet.at(_mode).at(_color);
}

std::string Theme::Color::asHexString() const
{
    std::ostringstream oss;
    oss << "#" << std::uppercase << std::hex << std::setw(6) << std::setfill('0') << (asHex() & 0xFFFFFF);
    return oss.str();
}

Theme::Color Theme::newColor(ThemeColor color)
{
    return Color(Theme::_lastConfig.mode, color);
}

juce::Font Theme::newFont(FontWeight style, FontSize size)
{
    return _activeFonts.at(style).withHeight(getFontSizeInPixels(size));
}

juce::Font Theme::newFont(FontWeight style, float textHeight)
{
    return _activeFonts.at(style).withHeight(textHeight);
}

juce::Font Theme::newFont(FontFamily family, FontWeight weight, FontSize size)
{
    const FontSet& fonts = family == FontFamily::MONOSPACED ? _activeMonospaceFonts : _activeFonts;
    return fonts.at(weight).withHeight(getFontSizeInPixels(size, family));
}

float Theme::getFontSizeInPixels(const FontSize size, const FontFamily family)
{
    if (family == FontFamily::MONOSPACED)
        return monospacedFontSizesToPixels.at(size);
    return defaultFontSizesToPixels.at(size);
}

}
