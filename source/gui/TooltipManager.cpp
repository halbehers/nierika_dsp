#include "../../include/gui/TooltipManager.h"

namespace nierika::gui
{

void TooltipManager::registerComponent(juce::Component& component, const std::string& tooltipText)
{
    const std::string id = component.getComponentID().toStdString();
    const Tooltip tooltip = { id, component.getName().toStdString(), tooltipText };

    _tooltipsByComponentID[id] = tooltip;
    setupComponent(component);
}

void TooltipManager::registerComponent(Component& component)
{
    registerComponent(component, component.getTooltip());
}

void TooltipManager::unregisterComponent(juce::Component& component)
{
    const std::string id = component.getComponentID().toStdString();

    _tooltipsByComponentID.erase(id);
    component.removeMouseListener(this);
    _registeredComponents.erase(&component);
}

void TooltipManager::setupComponent(juce::Component& component)
{
    if (!_registeredComponents.contains(&component))
    {
        component.addMouseListener(this, true);
        _registeredComponents.insert(&component);
    }
}

void TooltipManager::clearCurrentTooltip()
{
    _currentTooltip = _defaultTooltip;
}

void TooltipManager::mouseEnter(const juce::MouseEvent& event)
{
    const std::string id = event.eventComponent->getComponentID().toStdString();
    if (const auto it = _tooltipsByComponentID.find(id); it != _tooltipsByComponentID.end())
    {
        _currentTooltip = it->second.text;
        notifyListeners();
    }
}

void TooltipManager::mouseExit(const juce::MouseEvent& event)
{
    (void) event;

    _currentTooltip = _defaultTooltip;
    notifyListeners();
}

std::string TooltipManager::getCurrentTooltip() const
{
    return _currentTooltip;
}

void TooltipManager::setDefaultTooltip(const std::string& tooltip)
{
    _defaultTooltip = tooltip;
    if (_currentTooltip.empty())
        _currentTooltip = _defaultTooltip;
}

void TooltipManager::addListener(Listener* listener)
{
    _listeners.push_back(listener);
}

void TooltipManager::removeListener(Listener* listener)
{
    std::erase(_listeners, listener);
}

void TooltipManager::notifyListeners() const {
    for (auto* listener : _listeners)
    {
        listener->tooltipChanged(_currentTooltip);
    }
}

}
