#include "../../../include/gui/element/ComboBox.h"

#include <algorithm>

namespace nierika::gui::element
{

ComboBox::ComboBox(const std::string& identifier):
    Component(identifier)
{
    setup();
}

void ComboBox::setup()
{
    addAndMakeVisible(_comboBox);

    _comboBox.setLookAndFeel(&_lookAndFeel);
    _comboBox.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    _comboBox.setColour(juce::ComboBox::ColourIds::textColourId, Theme::newColor(Theme::ThemeColor::TEXT).asJuce());

    _comboBox.onChange = [this]()
    {
        for (auto* listener : _listeners)
            listener->onSelectionChanged(getID(), _comboBox.getSelectedId());
    };
}

void ComboBox::resized()
{
    Component::resized();
    const auto bounds = getLocalBounds();
    const auto height = static_cast<int>(Theme::resolveHeight(_heightType, static_cast<float>(bounds.getHeight())));
    _comboBox.setBounds(bounds.withSizeKeepingCentre(bounds.getWidth(), height));
}

void ComboBox::addItem(const juce::String& newItemText, int newItemId)
{
    _comboBox.addItem(newItemText, newItemId);
}

void ComboBox::setSelectedId(int newItemId, juce::NotificationType notification)
{
    _comboBox.setSelectedId(newItemId, notification);
}

int ComboBox::getSelectedId() const
{
    return _comboBox.getSelectedId();
}

void ComboBox::addOnValueChangedListener(OnValueChangedListener* listener)
{
    _listeners.push_back(listener);
}

void ComboBox::removeListener(OnValueChangedListener* listener)
{
    _listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
}

void ComboBox::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    _comboBox.setColour(juce::ComboBox::ColourIds::textColourId, Theme::newColor(Theme::ThemeColor::TEXT).asJuce());

    resized();

    Component::changeListenerCallback(source);
}

}
