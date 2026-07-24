#pragma once

#include <string>
#include <optional>

#include "Theme.h"
#include "Spacing.h"
#include "../utils/UID.h"

namespace nierika::gui
{

class Component : public juce::Component, public juce::ChangeListener
{
public:
    explicit Component(const std::string& identifier = utils::UID::generate_v4(), const std::string& name = "", const std::string& tooltip = "");
    ~Component() override;

    std::string getID() const { return getComponentID().toStdString(); }

    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    enum Alignment { START, END, CENTER };

    void setFixedHeight(float height, bool shouldResize = true) { _fixedHeight = height; if (shouldResize) resized(); }
    void resetFixedHeight() { _fixedHeight = -1.f; resized(); }
    [[nodiscard]] float getFixedHeight() const { return _fixedHeight; }

    void setFixedWidth(float width, bool shouldResize = true) { _fixedWidth = width; if (shouldResize) resized(); }
    void resetFixedWidth() { _fixedWidth = -1.f; resized(); }
    [[nodiscard]] float getFixedWidth() const { return _fixedWidth; }

    // Ignored unless a fixed height is set.
    void setVerticalAlignment(Alignment alignment) { _verticalAlignment = alignment; resized(); }
    [[nodiscard]] Alignment getVerticalAlignment() const { return _verticalAlignment; }

    // Ignored unless a fixed width is set.
    void setHorizontalAlignment(Alignment alignment) { _horizontalAlignment = alignment; resized(); }
    [[nodiscard]] Alignment getHorizontalAlignment() const { return _horizontalAlignment; }

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

    int getOuterX();
    int getOuterY();
    int getOuterWidth();
    int getOuterHeight();

private:
    std::string _tooltip;
    bool _tooltipEnabled = false;

    struct Border
    {
        bool display = false;
        juce::Colour color;
        float lineThickness;
        float radius;
        float alpha = 1.f;
        std::optional<Theme::ThemeColor> themeColor;
    } _border;

    struct Background
    {
        bool display = false;
        juce::Colour color;
        float radius;
        juce::ColourGradient gradient;
        bool isGradient;
        float alpha = 1.f;
        std::optional<Theme::ThemeColor> themeColor;
    } _background;

    layout::Spacing<float> _margin { 0.f, 0.f, 0.f, 0.f };
    layout::Spacing<float> _fixedSizedMargin { 0.f, 0.f, 0.f, 0.f };
    layout::Spacing<float> _padding { 0.f, 0.f, 0.f, 0.f };

    float _fixedHeight = -1.f;
    float _fixedWidth = -1.f;
    Alignment _verticalAlignment = START;
    Alignment _horizontalAlignment = START;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Component)
};

}
