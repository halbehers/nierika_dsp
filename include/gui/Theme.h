#pragma once

#include <unordered_map>
#include <optional>
#include <string>

#define COLOR_WHITE 0xFFFFF8F8

#define COLOR_GRAY_50 0xFF808080
#define COLOR_GRAY_100 0xFFAEAEAE
#define COLOR_GRAY_200 0xFF6C6C6C
#define COLOR_GRAY_300 0xFF4A4446
#define COLOR_GRAY_400 0xFF2C2C2C

#define COLOR_BLUE_500 0xFF283F51
#define COLOR_BLUE_400 0xFF334E63
#define COLOR_BLUE_300 0xFF3A607E

#define COLOR_BLACK 0xFF000000


#define COLOR_TURQUOISE_200 0xFF53F4FB
#define COLOR_TURQUOISE_300 0xFF39B0C1

#define COLOR_RED_100 0xFFF96666
#define COLOR_ORANGE_100 0xFFF2A048

#define COLOR_PRIMARY COLOR_BLUE_400
#define COLOR_ACCENT COLOR_TURQUOISE_200
#define COLOR_TEXT COLOR_WHITE
#define COLOR_INVERTED_TEXT COLOR_BLACK
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

#define COLOR_BACKGROUND 0xFF1E1E1E
#define COLOR_SECONDARY_BACKGROUND 0xFFF2F2F2
#define COLOR_BORDER COLOR_LIGHTER_SHADE

#define DEFAULT_BORDER_RADIUS 17.f

namespace nierika::gui
{

class Theme
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
        BLACK,
        BACKGROUND,
        SECONDARY_BACKGROUND,
        BORDER,
        PRIMARY,
        ACCENT,
        TEXT,
        INVERTED_TEXT,
        DISABLED,
        DANGER,
        WARNING
    };

    enum FontWeight
    {
        BOLD,
        MEDIUM,
        REGULAR,
        LIGHT,
        THIN
    };

    enum FontSize
    {
        TITLE,
        SUBTITLE,
        HEADING,
        CAPTION,
        PARAGRAPH,
        LABEL,
        SMALL
    };

    enum class Mode
    {
        DARK,
        LIGHT
    };

    enum class FontFamily
    {
        DEFAULT,
        MONOSPACED
    };

    using Palette = std::unordered_map<ThemeColor, juce::Colour>;
    using PaletteSet = std::unordered_map<Mode, Palette>;
    using FontSet = std::unordered_map<FontWeight, juce::Font>;

    // Host-facing configuration. Everything is optional: an untouched Config
    // reproduces today's hardcoded Dark theme exactly.
    struct Config
    {
        Mode mode = Mode::DARK;
        std::optional<PaletteSet> customPaletteSet;
        PaletteSet colorOverrides;
        FontSet fontOverrides;
        FontSet monospaceFontOverrides;
        std::optional<float> borderRadius;
    };

    class Color
    {
    public:
        explicit Color(const Mode mode, const ThemeColor color);
        ~Color() = default;

        Color(const Color& other) = default;
        Color& operator=(const Color& other) = default;

        [[nodiscard]] juce::uint32 asHex() const;
        [[nodiscard]] juce::Colour asJuce() const;
        [[nodiscard]] std::string asHexString() const;

    private:
        Mode _mode;
        ThemeColor _color;
    };

    // One-call host configuration entry point.
    static void configure(const Config& config);

    // Fine-grained mutators. Each composes with whatever was last configured
    // (previous overrides are preserved) rather than resetting it.
    static void setMode(Mode mode);
    [[nodiscard]] static Mode getMode();
    static void setColor(const Mode mode, ThemeColor color, juce::Colour value);
    static void setPaletteSet(const PaletteSet& palette);
    static void setFont(FontWeight weight, juce::Font font);
    static void setFonts(const FontSet& fonts);
    static void setMonospaceFont(FontWeight weight, juce::Font font);
    static void setMonospaceFonts(const FontSet& fonts);
    static void setBorderRadius(float radius);
    [[nodiscard]] static float getBorderRadius();
    static void resetToDefaults();

    // Fires whenever the active palette/fonts change, so live components can
    // refresh cached theme-derived colours/fonts and repaint.
    [[nodiscard]] static juce::ChangeBroadcaster& getChangeBroadcaster();

    static Color newColor(ThemeColor color);
    [[nodiscard]] static juce::Font newFont(FontWeight weight, FontSize size = PARAGRAPH);
    [[nodiscard]] static juce::Font newFont(FontFamily family, FontWeight weight, FontSize size = PARAGRAPH);
    [[nodiscard]] static float getFontSizeInPixels(FontSize size, const FontFamily family = FontFamily::DEFAULT);

private:
    static PaletteSet buildPreset();
    static FontSet buildDefaultFontSet();
    static FontSet buildDefaultMonospaceFontSet();
    static void applyConfig(const Config& config);

    static Config _lastConfig;
    static PaletteSet _activePaletteSet;
    static FontSet _activeFonts;
    static FontSet _activeMonospaceFonts;
    static float _borderRadius;
    static std::unordered_map<FontSize, float> defaultFontSizesToPixels;
    static std::unordered_map<FontSize, float> monospacedFontSizesToPixels;
    static juce::ChangeBroadcaster _changeBroadcaster;

    friend class Color;
};

}
