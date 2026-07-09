#include "../../../include/gui/element/PrimaryTextButton.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

PrimaryTextButton::PrimaryTextButton(const std::string& identifier, const std::string& text):
    TextButton(identifier, text)
{
    setColour(juce::TextButton::buttonColourId, Theme::ThemeColor::ACCENT);
    setColour(juce::TextButton::textColourOnId, Theme::ThemeColor::FULL_SHADE);
    setColour(juce::TextButton::textColourOffId, Theme::ThemeColor::MEDIUM_SHADE);

    _button.setLookAndFeel(&_lookAndFeel);
}

}
