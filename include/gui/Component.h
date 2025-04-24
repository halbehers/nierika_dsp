#pragma once

#include <string>

#include "Spacing.h"
#include "../utils/UID.h"

namespace nierika::gui
{

class Component : public juce::Component
{
public:
    Component(const std::string& identifier = utils::UID::generate_v4(), const std::string& name = "");
    ~Component();

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
    void setMargin(const T marginLeft, const T marginTop, const T marginRight, const T marginBottom);
    template<typename T>
    void setMargin(const T horizontalMargin, const T verticalMargin);
    template<typename T>
    void setMargin(const T value);

    template<typename T>
    void setPadding(layout::Spacing<T> paddings);
    template<typename T>
    void setPadding(const T paddingLeft, const T paddingTop, const T paddingRight, const T paddingBottom);
    template<typename T>
    void setPadding(const T horizontalPadding, const T verticalPadding);
    template<typename T>
    void setPadding(const T value);

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
