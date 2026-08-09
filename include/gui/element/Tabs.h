#pragma once

#include "../Component.h"
#include "../layout/GridLayout.h"
#include "./TextButton.h"

namespace nierika::gui::element
{

class Tabs: public Component, public TextButton::OnClickListener
{
public:
    struct OnTabChangedListener
    {
        virtual ~OnTabChangedListener() = default;
        virtual void onTabChanged(const std::string& newSelectedTabID) = 0;
    };

    Tabs();
    explicit Tabs(const std::string& identifier);
    ~Tabs() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void addOnTabChangedListener(OnTabChangedListener* listener);
    void removeListener(OnTabChangedListener* listener);

    void onButtonClick(const std::string& componentID) override;

    std::string getSelectedTabID() const { return _selectedTabID; }
    void setSelectedTabID(const std::string& id);

    void addTab(const std::string& id, const std::string& name, const std::string& tooltip = "", bool isSelected = false);
    void removeTab(const std::string& id);

    void setTabName(const std::string& id, const std::string& name);
    std::string getTabName(const std::string& id) const;
    std::string getSelectedTabName() const;
    void setTabTooltip(const std::string& id, const std::string& tooltip);
    std::string getTabTooltip(const std::string& id) const;
    std::string getSelectedTabTooltip() const;

    int count() const { return static_cast<int>(_tabs.size()); }

    void reset();

    void setBackgroundColour(juce::Colour colour);
    void resetBackgroundColour();
    juce::Colour getBackgroundColour() const;

    void setSelectedBackgroundColour(juce::Colour colour);
    void resetSelectedBackgroundColour();
    juce::Colour getSelectedBackgroundColour() const;

    void setBorderColour(juce::Colour colour);
    void resetBorderColour();
    juce::Colour getBorderColour() const;

    void setSelectedBorderColour(juce::Colour colour);
    void resetSelectedBorderColour();
    juce::Colour getSelectedBorderColour() const;

    void setBorderRadius(float radius);
    void resetBorderRadius();
    float getBorderRadius() const;

    void setFontSize(Theme::FontSize size);
    void resetFontSize();
    Theme::FontSize getFontSize() const;

    void setFontWeight(Theme::FontWeight weight);
    void resetFontWeight();
    Theme::FontWeight getFontWeight() const;

    void setFont(Theme::FontWeight weight, Theme::FontSize size);
    void resetFont();

    void setHeightType(Theme::HeightType type);
    [[nodiscard]] Theme::HeightType getHeightType() const { return _heightType; }

    void setTabDesign(Theme::TabDesign design);
    [[nodiscard]] Theme::TabDesign getTabDesign() const { return _design; }

    void setTextColour(juce::Colour colour);
    void resetTextColour();
    juce::Colour getTextColour() const;

    void setSelectedTextColour(juce::Colour colour);
    void resetSelectedTextColour();
    juce::Colour getSelectedTextColour() const;

private:
    layout::GridLayout<Component> _layout;
    std::vector<OnTabChangedListener*> _listeners;

    std::vector<std::unique_ptr<TextButton>> _tabs;

    std::string _selectedTabID;

    std::optional<juce::Colour> _backgroundOverride = std::nullopt;
    std::optional<juce::Colour> _selectedBackgroundOverride = std::nullopt;
    std::optional<juce::Colour> _borderOverride = std::nullopt;
    std::optional<juce::Colour> _selectedBorderOverride = std::nullopt;
    float _borderRadiusOverride = -1.f;
    std::optional<Theme::FontSize> _fontSizeOverride = std::nullopt;
    std::optional<Theme::FontWeight> _fontWeightOverride = std::nullopt;
    Theme::HeightType _heightType = Theme::HeightType::AUTO;
    std::optional<juce::Colour> _textColourOverride = std::nullopt;
    std::optional<juce::Colour> _selectedTextColourOverride = std::nullopt;
    Theme::TabDesign _design = Theme::TabDesign::BUTTON;

    void setup();

    void initLayout();

    void applyStyle(TextButton& tab) const;

    void applyDesignLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tabs)
};

}
 
