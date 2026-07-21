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
    _button.setMouseCursor(isEnabled() ? juce::MouseCursor::PointingHandCursor : juce::MouseCursor::NormalCursor);

    _button.onClick = [this]()
    {
        for (auto listener : _listeners)
            listener->onButtonClick(getID());
    };
}

void SVGButton::resized()
{
    Component::resized();
    const auto bounds = getLocalBounds();
    if (_clickableSurface == helpers::ClickableSurface::ELEMENT_BOUNDARIES)
    {
        const auto size = static_cast<int>(getIconSize());
        _button.setBounds(bounds.withSizeKeepingCentre(size, size));
    }
    else
    {
        _button.setBounds(bounds);
    }
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

void SVGButton::setEnabled(bool shouldBeEnabled)
{
    juce::Component::setEnabled(shouldBeEnabled);

    _button.setMouseCursor(shouldBeEnabled ? juce::MouseCursor::PointingHandCursor : juce::MouseCursor::NormalCursor);
}

}
