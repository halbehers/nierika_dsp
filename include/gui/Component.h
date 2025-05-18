#pragma once

#include <string>

#include "Theme.h"
#include "Spacing.h"
#include "../utils/UID.h"

namespace nierika::gui
{

class Component : public juce::Component
{
public:
    explicit Component(const std::string& identifier = utils::UID::generate_v4(), const std::string& name = "", const std::string& tooltip = "");
    ~Component() override;

    std::string getID() const { return getComponentID().toStdString(); }

    void paint(juce::Graphics& g) override;

    void setTooltip(const std::string& text);
    const std::string& getTooltip() const;

    void setTooltipEnabled(bool enabled);
    bool isTooltipEnabled() const;

    void displayBorder(Theme::ThemeColor color, float lineThickness = 1.f, float radius = 0.f, float alpha = 1.f);
    void displayBorder(juce::Colour color, float lineThickness = 1.f, float radius = 0.f);
    void hideBorder();

    void displayBackground(Theme::ThemeColor color, float radius = 0.f, float alpha = 1.f);
    void displayBackground(juce::Colour color, float radius = 0.f);
    void displayBackground(juce::ColourGradient gradient, float radius = 0.f);
    void hideBackground();

    template<typename T>
    void setMargin(layout::Spacing<T> margins);
    template<typename T>
    void setMargin(T marginLeft, T marginTop, T marginRight, T marginBottom);
    template<typename T>
    void setMargin(T horizontalMargin, T verticalMargin);
    template<typename T>
    void setMargin(T value);
    template<typename T>
    void withTopMargin(T value);
    template<typename T>
    void withLeftMargin(T value);
    template<typename T>
    void withRightMargin(T value);
    template<typename T>
    void withBottomMargin(T value);
    layout::Spacing<float> getMargin() const { return _margin; }

    template<typename T>
    void setPadding(layout::Spacing<T> padding);
    template<typename T>
    void setPadding(T paddingLeft, T paddingTop, T paddingRight, T paddingBottom);
    template<typename T>
    void setPadding(T horizontalPadding, T verticalPadding);
    template<typename T>
    void setPadding(T value);
    template<typename T>
    void withTopPadding(T value);
    template<typename T>
    void withLeftPadding(T value);
    template<typename T>
    void withRightPadding(T value);
    template<typename T>
    void withBottomPadding(T value);
    layout::Spacing<float> getPadding() const { return _margin; }

    juce::Rectangle<int> getLocalBounds();
    juce::Rectangle<int> getOuterLocalBounds();

private:
    std::string _tooltip;
    bool _tooltipEnabled = false;

    struct Border
    {
        bool display = false;
        juce::Colour color;
        float lineThickness;
        float radius;
    } _border;

    struct Background
    {
        bool display = false;
        juce::Colour color;
        float radius;
        juce::ColourGradient gradient;
        bool isGradient;
    } _background;

    layout::Spacing<float> _margin { 0.f, 0.f, 0.f, 0.f };
    layout::Spacing<float> _padding { 0.f, 0.f, 0.f, 0.f };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Component)
};

}
