#include "../../../include/gui/element/SVGToggle.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/EmbeddedFonts.h"
#include "../../../include/gui/Helpers.h"

namespace nierika::gui::element
{

SVGToggle::SVGToggle(const std::string& identifier, const char* svgBinary):
    Component(identifier),
    _lookAndFeel(svgBinary)
{
    setup();
}

SVGToggle::SVGToggle(dsp::ParameterManager& parameterManager, const std::string& parameterID, const char* svgBinary):
    Component(parameterID, parameterManager.getParameterDisplayName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _lookAndFeel(svgBinary)
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

    _button.onClick = [this]()
    {
        for (auto listener : _listeners)
            listener->onToggleValueChanged(getID(), _button.getToggleState());
    };
}

void SVGToggle::resized()
{
    Component::resized();
    _button.setBounds(getLocalBounds());
}

void SVGToggle::paint(juce::Graphics& g)
{
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

}
