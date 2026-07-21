#pragma once

#include "../Component.h"
#include "../laf/ComboBox.h"

namespace nierika::gui::element
{

class ComboBox: public Component
{
public:
    struct OnValueChangedListener
    {
        virtual ~OnValueChangedListener() = default;
        virtual void onSelectionChanged(const std::string& componentID, int selectedId) = 0;
    };

    explicit ComboBox(const std::string& identifier);
    ~ComboBox() override = default;

    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void addItem(const juce::String& newItemText, int newItemId);
    void setSelectedId(int newItemId, juce::NotificationType notification = juce::sendNotification);
    [[nodiscard]] int getSelectedId() const;

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeListener(OnValueChangedListener* listener);

    void setSelectedInvertedTextColor(bool hasInvertedTextColorOnSelected) { _hasInvertedTextColorOnSelected = hasInvertedTextColorOnSelected; }
    bool hasInvertedTextColorOnSelected() const { return _hasInvertedTextColorOnSelected; }

    void setHeightType(Theme::HeightType type) { _heightType = type; resized(); _comboBox.lookAndFeelChanged(); }
    [[nodiscard]] Theme::HeightType getHeightType() const { return _heightType; }

    int getHeight() { return static_cast<int>(Theme::resolveHeight(_heightType, static_cast<float>(getLocalBounds().getHeight()))); }
    
    void setBackgroundColour(juce::Colour colour) { _backgroundOverride = colour; }
    void resetBackgroundColour() { _backgroundOverride = juce::Colour(); }
    juce::Colour getBackgroundColour() const { return _backgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce()); }

    void setBorderColour(juce::Colour colour) { _borderOverride = colour; }
    void resetBorderColour() { _borderOverride = juce::Colour(); }
    juce::Colour getBorderColour() const { return _borderOverride.value_or(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.2f)); }

    void setBorderRadius(float radius) { _borderRadiusOverride = radius; }
    void resetBorderRadius() { _borderRadiusOverride = -1; }
    float getBorderRadius() const { return _borderRadiusOverride >= 0 ? _borderRadiusOverride : Theme::getBorderRadius(); }

    void setRounded(bool isRounded) { setBorderRadius(getHeight() / 2); }

    void setTextHeight(int textHeight) { _textHeightOverride = textHeight; _comboBox.lookAndFeelChanged(); }
    void setTextHeight(const Theme::FontSize size) { _textHeightOverride = Theme::getFontSizeInPixels(size); _comboBox.lookAndFeelChanged(); }
    void resetTextHeight() { _textHeightOverride = -1; _comboBox.lookAndFeelChanged(); }
    float getTextHeight() const { return _textHeightOverride >= 0.f ? _textHeightOverride : Theme::getFontSizeInPixels(_heightType == Theme::HeightType::THIN ? Theme::SMALL : Theme::PARAGRAPH); }

private:
    laf::ComboBox _lookAndFeel { *this };
    juce::ComboBox _comboBox {};
    bool _hasInvertedTextColorOnSelected = false;
    Theme::HeightType _heightType = Theme::HeightType::AUTO;

    std::vector<OnValueChangedListener*> _listeners;

    std::optional<juce::Colour> _backgroundOverride = std::nullopt;
    std::optional<juce::Colour> _borderOverride = std::nullopt;
    float _textHeightOverride = -1.f;
    float _borderRadiusOverride = -1;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBox)
};

}
