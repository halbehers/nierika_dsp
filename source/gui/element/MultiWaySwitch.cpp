#include "../../../include/gui/element/MultiWaySwitch.h"
#include "../../../include/gui/Theme.h"

#include <algorithm>
#include <cmath>

namespace nierika::gui::element
{

MultiWaySwitch::MultiWaySwitch(const std::string& identifier, std::vector<std::string> labels):
    Component(identifier),
    _labels(std::move(labels))
{
    jassert(_labels.size() >= 2);

    setInterceptsMouseClicks(true, false);
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void MultiWaySwitch::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto localBounds = getLocalBounds();
    const auto height = getHeight();
    const auto bounds = localBounds.withSizeKeepingCentre(localBounds.getWidth(), height).toFloat();
    const auto radius = getBorderRadius();
    const auto numSegments = static_cast<int>(_labels.size());

    g.setColour(getBackgroundColour());
    g.fillRoundedRectangle(bounds, radius);
    g.setColour(getBorderColour());
    g.drawRoundedRectangle(bounds, radius, 1.f);

    const auto segmentWidth = bounds.getWidth() / static_cast<float>(numSegments);

    constexpr float thumbMargin = 2.f;
    const auto selectedBounds = juce::Rectangle<float>(
        bounds.getX() + segmentWidth * _thumbPosition + thumbMargin,
        bounds.getY() + thumbMargin,
        segmentWidth - thumbMargin * 2.f,
        bounds.getHeight() - thumbMargin * 2.f);

    g.setColour(Theme::newColor(Theme::ACCENT).asJuce());
    g.fillRoundedRectangle(selectedBounds, radius - thumbMargin);

    g.setFont(Theme::newFont(Theme::MEDIUM, Theme::SMALL));

    for (int i = 0; i < numSegments; ++i)
    {
        const auto segmentBounds = juce::Rectangle<float>(bounds.getX() + segmentWidth * (float) i, bounds.getY(), segmentWidth, bounds.getHeight());
        g.setColour(Theme::newColor(i == _selectedIndex ? (hasInvertedTextColorOnSelected() ? Theme::INVERTED_TEXT : Theme::TEXT) : Theme::DISABLED).asJuce());
        g.drawFittedText(_labels[(size_t) i], segmentBounds.toNearestInt(), juce::Justification::centred, 1);
    }
}

void MultiWaySwitch::mouseUp(const juce::MouseEvent& event)
{
    if (!event.mouseWasClicked())
        return;

    const auto numSegments = static_cast<int>(_labels.size());
    const auto segmentWidth = (float) getWidth() / static_cast<float>(numSegments);
    const auto clickedIndex = juce::jlimit(0, numSegments - 1, (int) (event.position.x / segmentWidth));

    setSelectedIndex(clickedIndex);
}

void MultiWaySwitch::setSelectedIndex(int index, juce::NotificationType notification)
{
    if (_selectedIndex == index)
        return;

    _selectedIndex = index;
    startTimerHz(60);

    if (notification == juce::sendNotification)
        for (auto* listener : _listeners)
            listener->onSelectionChanged(getID(), _selectedIndex);
}

void MultiWaySwitch::timerCallback()
{
    constexpr float smoothing = 0.25f;
    constexpr float snapThreshold = 0.001f;

    const auto target = static_cast<float>(_selectedIndex);
    _thumbPosition += (target - _thumbPosition) * smoothing;

    if (std::abs(target - _thumbPosition) < snapThreshold)
    {
        _thumbPosition = target;
        stopTimer();
    }

    repaint();
}

void MultiWaySwitch::addOnValueChangedListener(OnValueChangedListener* listener)
{
    _listeners.push_back(listener);
}

void MultiWaySwitch::removeListener(OnValueChangedListener* listener)
{
    _listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
}

}
