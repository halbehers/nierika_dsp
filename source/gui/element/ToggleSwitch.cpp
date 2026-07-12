#include "../../../include/gui/element/ToggleSwitch.h"
#include "../../../include/gui/Theme.h"

#include <algorithm>
#include <cmath>

namespace nierika::gui::element
{

ToggleSwitch::ToggleSwitch(const std::string& identifier):
    Component(identifier)
{
    setInterceptsMouseClicks(true, false);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void ToggleSwitch::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds().toFloat();
    const auto trackColor = Theme::newColor(_isOn ? Theme::ACCENT : Theme::SECONDARY_BACKGROUND).asJuce();
    const auto thumbColor = Theme::newColor(_isOn ? hasInvertedColorOnSelected() ? Theme::INVERTED_TEXT : Theme::TEXT : Theme::TEXT).asJuce();

    g.setColour(trackColor);
    g.fillRoundedRectangle(bounds, bounds.getHeight() / 2.f);

    constexpr float thumbMargin = 2.f;
    const auto thumbDiameter = bounds.getHeight() - thumbMargin * 2.f;
    const auto thumbOffX = bounds.getX() + thumbMargin;
    const auto thumbOnX = bounds.getRight() - thumbDiameter - thumbMargin;
    const auto thumbX = thumbOffX + (thumbOnX - thumbOffX) * _thumbPosition;

    g.setColour(thumbColor);
    g.fillEllipse(thumbX, bounds.getY() + thumbMargin, thumbDiameter, thumbDiameter);
}

void ToggleSwitch::mouseUp(const juce::MouseEvent& event)
{
    if (!event.mouseWasClicked())
        return;

    setToggleState(!_isOn);
}

void ToggleSwitch::setToggleState(bool isOn, juce::NotificationType notification)
{
    if (_isOn == isOn)
        return;

    _isOn = isOn;
    startTimerHz(60);

    if (notification == juce::sendNotification)
        for (auto* listener : _listeners)
            listener->onToggleValueChanged(getID(), _isOn);
}

void ToggleSwitch::timerCallback()
{
    constexpr float smoothing = 0.25f;
    constexpr float snapThreshold = 0.001f;

    const auto target = _isOn ? 1.f : 0.f;
    _thumbPosition += (target - _thumbPosition) * smoothing;

    if (std::abs(target - _thumbPosition) < snapThreshold)
    {
        _thumbPosition = target;
        stopTimer();
    }

    repaint();
}

void ToggleSwitch::addOnValueChangedListener(OnValueChangedListener* listener)
{
    _listeners.push_back(listener);
}

void ToggleSwitch::removeListener(OnValueChangedListener* listener)
{
    _listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
}

}
