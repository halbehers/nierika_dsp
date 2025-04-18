#pragma once

#include <string>

namespace nierika::gui
{

class Component : public juce::Component
{
public:
    Component(const std::string& identifier, const std::string& name = "", const std::string& tooltip = "");
    ~Component();
    
    void setTooltip(const std::string& text);
    const std::string& getTooltip() const;
    
private:
    std::string _tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Component)
};

}
