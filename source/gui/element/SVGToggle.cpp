#include "../../../include/gui/element/SVGToggle.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/EmbeddedFonts.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::element
{

SVGToggle::SVGToggle(const std::string& identifier, const char* svgBinary):
    Component(identifier),
    _svgBinary(svgBinary)
{
    setup();
}

SVGToggle::SVGToggle(dsp::ParameterManager& parameterManager, const std::string& parameterID, const char* svgBinary):
    Component(parameterID, parameterManager.getParameterDisplayName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _svgBinary(svgBinary)
{
    setup();

    _button.setToggleState(parameterManager.getParameterDefaultValue(parameterID, true), juce::NotificationType::dontSendNotification);

    if (parameterID != "")
        _attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameterManager.getState(), parameterID, _button);
}

void SVGToggle::setup()
{
    addAndMakeVisible(_button);

    _button.setLookAndFeel(&_lookAndFeel);
    _button.setToggleState(true, juce::NotificationType::dontSendNotification);
    _button.setMouseCursor(isEnabled() ? juce::MouseCursor::PointingHandCursor : juce::MouseCursor::NormalCursor);

    _button.onClick = [this]()
    {
        for (auto listener : _listeners)
            listener->onToggleValueChanged(getID(), _button.getToggleState());
    };
}

void SVGToggle::resized()
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

void SVGToggle::paint(juce::Graphics& g)
{
    (void) g;
}

bool SVGToggle::getToggleState() const
{
    return _button.getToggleState();
}

void SVGToggle::setHelpText(const std::string& helpText)
{
    return _button.setHelpText(helpText);
}

void SVGToggle::addOnValueChangedListener(OnValueChangedListener* listener)
{
    _listeners.push_back(listener);
}

void SVGToggle::removeListener(OnValueChangedListener* listener)
{
    _listeners.erase(
        std::remove(_listeners.begin(), _listeners.end(), listener),
        _listeners.end()
    );
}

void SVGToggle::setEnabled(bool shouldBeEnabled)
{
    juce::Component::setEnabled(shouldBeEnabled);

    _button.setMouseCursor(shouldBeEnabled ? juce::MouseCursor::PointingHandCursor : juce::MouseCursor::NormalCursor);
}

}
