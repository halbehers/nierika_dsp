#pragma once

#include "../Component.h"
#include "../laf/TextButton.h"

namespace nierika::gui::element
{

class TextButton: public Component
{
public:    
    struct OnClickListener
    {
        virtual ~OnClickListener() = default;
        virtual void onButtonClick(const std::string& componentID) = 0;
    };

    enum class IconPosition
    {
        LEFT,
        RIGHT
    };

    TextButton(const std::string& identifier, const std::string& text);
    ~TextButton() override = default;

    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void addOnClickListener(OnClickListener* listener);
    void removeListener(OnClickListener* listener);

    void setButtonText(const std::string& text);
    void setHelpText(const std::string& helpText);

    void setIsSelected(const bool isSelected) { _isSelected = isSelected; }
    bool isSelected() const { return _isSelected; }
    
    void setColour(int colourID, juce::Colour newColour);
    void setColour(int colourID, Theme::ThemeColor color);

    void setIcon(const char* svgBinary, IconPosition position = IconPosition::LEFT);
    void resetIcon();
    bool hasIcon() const { return _iconBinary != nullptr; }
    IconPosition getIconPosition() const { return _iconPosition; }
    const char* getIconBinary() const { return _iconBinary; }
    float getIconSize() const { return static_cast<float>(_button.getHeight()) * 0.4f; }
    void setGap(float gap) { _gap = gap; }
    float getGap() const { return _gap; }
    void setInvertedTextColor(bool hasInvertedTextColor) { _hasInvertedTextColor = hasInvertedTextColor; }
    bool hasInvertedTextColor() const { return _hasInvertedTextColor; }

    void setHeightType(Theme::HeightType type) { _heightType = type; resized(); }
    [[nodiscard]] Theme::HeightType getHeightType() const { return _heightType; }

    void setTabDesign(Theme::TabDesign design) { _tabDesign = design; repaint(); }
    [[nodiscard]] Theme::TabDesign getTabDesign() const { return _tabDesign; }

    // TabDesign::TAB only: this tab sits directly next to the selected tab, so its near top
    // corner is carved out with a concave notch (filled with notchColour) instead of being
    // square - see laf::TextButton::drawButtonBackground for how it's actually drawn.
    void setNotches(bool hasLeftNotch, bool hasRightNotch, juce::Colour notchColour)
    {
        _hasLeftNotch = hasLeftNotch;
        _hasRightNotch = hasRightNotch;
        _notchColour = notchColour;
        repaint();
    }
    [[nodiscard]] bool hasLeftNotch() const { return _hasLeftNotch; }
    [[nodiscard]] bool hasRightNotch() const { return _hasRightNotch; }
    [[nodiscard]] juce::Colour getNotchColour() const { return _notchColour; }

    void setBackgroundColour(juce::Colour colour) { _backgroundOverride = colour; repaint(); }
    void resetBackgroundColour() { _backgroundOverride = juce::Colour(); repaint(); }
    juce::Colour getBackgroundColour() const { return _backgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce()); }

    void setSelectedBackgroundColour(juce::Colour colour) { _selectedBackgroundOverride = colour; repaint(); }
    void resetSelectedBackgroundColour() { _selectedBackgroundOverride = juce::Colour(); repaint(); }
    juce::Colour getSelectedBackgroundColour() const { return _selectedBackgroundOverride.value_or(getBackgroundColour()); }

    void setHoverBackgroundColour(juce::Colour colour) { _hoverBackgroundOverride = colour; repaint(); }
    void resetHoverBackgroundColour() { _hoverBackgroundOverride = juce::Colour(); repaint(); }
    juce::Colour getHoverBackgroundColour() const { return _hoverBackgroundOverride.value_or((isSelected() ? getSelectedBackgroundColour() : getBackgroundColour()).withAlpha(0.9f)); }

    void setBorderColour(juce::Colour colour) { _borderOverride = colour; repaint(); }
    void resetBorderColour() { _borderOverride = juce::Colour(); repaint(); }
    juce::Colour getBorderColour() const { return _borderOverride.value_or(juce::Colours::transparentBlack); }

    void setSelectedBorderColour(juce::Colour colour) { _selectedBorderOverride = colour; repaint(); }
    void resetSelectedBorderColour() { _selectedBorderOverride = juce::Colour(); repaint(); }
    juce::Colour getSelectedBorderColour() const { return _selectedBorderOverride.value_or(getBorderColour()); }

    void setBorderRadius(float radius) { _borderRadiusOverride = radius; repaint(); }
    void resetBorderRadius() { _borderRadiusOverride = -1.f; repaint(); }
    float getBorderRadius() const { return _borderRadiusOverride >= 0.f ? _borderRadiusOverride : 8.f; }

    void setFontSize(Theme::FontSize size) { _fontSizeOverride = size; repaint(); }
    void resetFontSize() { _fontSizeOverride = std::nullopt; repaint(); }
    Theme::FontSize getFontSize() const { return _fontSizeOverride.value_or(Theme::CAPTION); }

    void setFontWeight(Theme::FontWeight weight) { _fontWeightOverride = weight; repaint(); }
    void resetFontWeight() { _fontWeightOverride = std::nullopt; repaint(); }
    Theme::FontWeight getFontWeight() const { return _fontWeightOverride.value_or(Theme::REGULAR); }

    void setFont(Theme::FontWeight weight, Theme::FontSize size) { setFontWeight(weight); setFontSize(size); }
    void resetFont() { resetFontWeight(); resetFontSize(); }

    void setTextColour(juce::Colour colour) { _textColourOverride = colour; repaint(); }
    void resetTextColour() { _textColourOverride = juce::Colour(); repaint(); }
    juce::Colour getTextColour() const { return _textColourOverride.value_or(Theme::newColor(hasInvertedTextColor() ? Theme::ThemeColor::INVERTED_TEXT : Theme::ThemeColor::TEXT).asJuce()); }

    void setSelectedTextColour(juce::Colour colour) { _selectedTextColourOverride = colour; repaint(); }
    void resetSelectedTextColour() { _selectedTextColourOverride = juce::Colour(); repaint(); }
    juce::Colour getSelectedTextColour() const { return _selectedTextColourOverride.value_or(getTextColour()); }

protected:
    juce::TextButton _button {};

private:
    laf::TextButton _lookAndFeel { *this };
    bool _isSelected = true;

    std::vector<OnClickListener*> _listeners;

    const char* _iconBinary = nullptr;
    IconPosition _iconPosition = IconPosition::LEFT;
    float _gap = 8.f;

    bool _hasInvertedTextColor = false;
    Theme::HeightType _heightType = Theme::HeightType::AUTO;
    Theme::TabDesign _tabDesign = Theme::TabDesign::BUTTON;
    bool _hasLeftNotch = false;
    bool _hasRightNotch = false;
    juce::Colour _notchColour = juce::Colours::transparentBlack;

    std::optional<juce::Colour> _backgroundOverride = std::nullopt;
    std::optional<juce::Colour> _selectedBackgroundOverride = std::nullopt;
    std::optional<juce::Colour> _hoverBackgroundOverride = std::nullopt;
    std::optional<juce::Colour> _borderOverride = std::nullopt;
    std::optional<juce::Colour> _selectedBorderOverride = std::nullopt;
    float _borderRadiusOverride = -1.f;
    std::optional<Theme::FontSize> _fontSizeOverride = std::nullopt;
    std::optional<Theme::FontWeight> _fontWeightOverride = std::nullopt;
    std::optional<juce::Colour> _textColourOverride = std::nullopt;
    std::optional<juce::Colour> _selectedTextColourOverride = std::nullopt;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextButton)
};

}
 
