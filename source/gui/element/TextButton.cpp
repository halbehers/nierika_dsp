#include "../../../include/gui/element/TextButton.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/EmbeddedFonts.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::element
{

TextButton::TextButton(const std::string& identifier, const std::string& text):
    Component(identifier, text)
{
    setup();
}

TextButton::~TextButton()
{
}

void TextButton::setup()
{
    addAndMakeVisible(_button);

    _button.setLookAndFeel(&_lookAndFeel);
    _button.setButtonText(getName());

    _button.onClick = [this]()
    {
        for (auto listener : _listeners)
            listener->onButtonClick(getID());
    };
}

void TextButton::resized()
{
    Component::resized();
    _button.setBounds(getLocalBounds());
}

void TextButton::setHelpText(const std::string& helpText)
{
    return _button.setHelpText(helpText);
}

void TextButton::addOnClickListener(OnClickListener* listener)
{
    _listeners.push_back(listener);
}

void TextButton::removeListener(OnClickListener* listener)
{
    _listeners.erase(
        std::remove(_listeners.begin(), _listeners.end(), listener),
        _listeners.end()
    );
}

void TextButton::setButtonText(const std::string& text)
{
    _button.setButtonText(text);
}

void TextButton::setColour(int colourID, juce::Colour newColour)
{
    _button.setColour(colourID, newColour);
}

void TextButton::setColour(int colourID, Theme::ThemeColor color)
{
    _button.setColour(colourID, Theme::getInstance().getColor(color).asJuce());
}

}
