#pragma once

#include "../Component.h"
#include "../laf/SVGButton.h"

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

    void setIconSize(float size) { _iconSize = size; }
    void resetIconSize() { _iconSize = -1.f; }
    float getIconSize() const { return _iconSize >= 0.f ? _iconSize : static_cast<float>(juce::jmin(getWidth(), getHeight())); }

private:
    laf::SVGButton _lookAndFeel { *this };
    juce::DrawableButton _button { "button", juce::DrawableButton::ButtonStyle::ImageFitted };

    const char* _svgBinary;
    float _iconSize = -1.f;

    std::vector<OnClickListener*> _listeners;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SVGButton)
};

}
