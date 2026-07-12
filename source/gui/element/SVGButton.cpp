#include "../../../include/gui/element/SVGButton.h"

namespace nierika::gui::element
{

SVGButton::SVGButton(const std::string& identifier, const char* svgBinary):
    Component(identifier),
    _svgBinary(svgBinary)
{
    setup();
}

void SVGButton::setup()
{
    addAndMakeVisible(_button);

    _button.setLookAndFeel(&_lookAndFeel);
    _button.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    _button.onClick = [this]()
    {
        for (auto listener : _listeners)
            listener->onButtonClick(getID());
    };
}

void SVGButton::resized()
{
    Component::resized();
    _button.setBounds(getLocalBounds());
}

void SVGButton::paint(juce::Graphics& g)
{
    (void) g;
}

void SVGButton::setHelpText(const std::string& helpText)
{
    _button.setHelpText(helpText);
}

void SVGButton::addOnClickListener(OnClickListener* listener)
{
    _listeners.push_back(listener);
}

void SVGButton::removeListener(OnClickListener* listener)
{
    _listeners.erase(
        std::remove(_listeners.begin(), _listeners.end(), listener),
        _listeners.end()
    );
}

}
