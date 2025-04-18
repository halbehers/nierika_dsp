#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "../utils/Singleton.h"
#include "Component.h"

namespace nierika::gui
{

class TooltipManager: public utils::Singleton<TooltipManager>, public juce::MouseListener
{
public:
    struct Tooltip
    {
        std::string componentID;
        std::string componentName;
        std::string text;
    };

    struct Listener
    {
        virtual ~Listener() = default;
        virtual void tooltipChanged(const std::string& newTooltip) = 0;
    };

    void registerComponent(juce::Component& component, const std::string& tooltip);
    void registerComponent(Component& component);
    void unregisterComponent(juce::Component& component);
    
    void addListener(Listener* listener);
    void removeListener(Listener* listener);
    
    std::string getCurrentTooltip() const;
    void setDefaultTooltip(const std::string& tooltip);

private:
    std::string _defaultTooltip;

    std::unordered_map<std::string, Tooltip> _tooltipsByComponentID;
    std::unordered_set<juce::Component*> _registeredComponents;
    std::vector<Listener*> _listeners;

    std::string _currentTooltip = _defaultTooltip;
    
    void setupComponent(juce::Component& component);
    void clearCurrentTooltip();

    // MouseListener overrides
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;

    void notifyListeners();
};

}
