#include "../../../include/gui/element/TextButton.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

TextButton::TextButton(const std::string& identifier, const std::string& text):
    Component(identifier, text)
{
    setup();
}

void TextButton::setup()
{
    addAndMakeVisible(_button);

    _button.setLookAndFeel(&_lookAndFeel);
    _button.setButtonText(getName());
    _button.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    _button.onClick = [this]()
    {
        for (auto listener : _listeners)
            listener->onButtonClick(getID());
    };
}

void TextButton::resized()
{
    Component::resized();
    const auto bounds = getLocalBounds();
    const auto height = static_cast<int>(Theme::resolveHeight(_heightType, static_cast<float>(bounds.getHeight())));
    _button.setBounds(bounds.withSizeKeepingCentre(bounds.getWidth(), height));
}

void TextButton::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    resized();
    Component::changeListenerCallback(source);
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
    _button.setColour(colourID, Theme::newColor(color).asJuce());
}

void TextButton::setIcon(const char* svgBinary, IconPosition position)
{
    _iconBinary = svgBinary;
    _iconPosition = position;
}

void TextButton::resetIcon()
{
    _iconBinary = nullptr;
    _iconPosition = IconPosition::LEFT;
}

}
