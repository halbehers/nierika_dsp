#pragma once

#include "../Component.h"
#include "../laf/SVGButton.h"
#include "../Helpers.h"

namespace nierika::gui::element
{

class SVGButton: public Component
{
public:
    struct OnClickListener
    {
        virtual ~OnClickListener() = default;
        virtual void onButtonClick(const std::string& componentID) = 0;
    };

    SVGButton(const std::string& identifier, const char* svgBinary);
    ~SVGButton() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void addOnClickListener(OnClickListener* listener);
    void removeListener(OnClickListener* listener);

    void setHelpText(const std::string& helpText);

    const char* getIconBinary() const { return _svgBinary; }
    void setIconBinary(const char* svgBinary) { _svgBinary = svgBinary; _button.lookAndFeelChanged(); }

    void setIconSize(float size) { _iconSize = size; resized(); }
    void resetIconSize() { _iconSize = -1.f; resized(); }
    float getIconSize() const { return _iconSize >= 0.f ? _iconSize : static_cast<float>(juce::jmin(getWidth(), getHeight())); }
   
    void setClickableSurface(const helpers::ClickableSurface clickableSurface) { _clickableSurface = clickableSurface; resized(); }
    helpers::ClickableSurface getClickableSurface() const { return _clickableSurface; }

    void setEnabled(bool shouldBeEnabled);

    void setColour(juce::Colour colour) { _colourOverride = colour; repaint(); }
    void setColour(Theme::ThemeColor colour) { _colourOverride = Theme::newColor(colour).asJuce(); repaint(); }
    void resetColour() { _colourOverride.reset(); repaint(); }
    juce::Colour getColour() const { return _colourOverride.value_or(Theme::newColor(Theme::ThemeColor::TEXT).asJuce()); }

    void setBackgroundColour(juce::Colour colour) { _backgroundColourOverride = colour; repaint(); }
    void setBackgroundColour(Theme::ThemeColor colour) { _backgroundColourOverride = Theme::newColor(colour).asJuce(); repaint(); }
    void resetBackgroundColour() { _backgroundColourOverride.reset(); repaint(); }
    juce::Colour getBackgroundColour() const { return _backgroundColourOverride.value_or(juce::Colours::transparentBlack); }

private:
    laf::SVGButton _lookAndFeel { *this };
    juce::DrawableButton _button { "button", juce::DrawableButton::ButtonStyle::ImageFitted };

    const char* _svgBinary;
    float _iconSize = -1.f;
    helpers::ClickableSurface _clickableSurface = helpers::ClickableSurface::ALL_AVAILABLE_AREA;
    std::optional<juce::Colour> _colourOverride;
    std::optional<juce::Colour> _backgroundColourOverride;

    std::vector<OnClickListener*> _listeners;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGButton)
};

}
