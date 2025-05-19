#pragma once

#include <string>

namespace nierika::gui::element
{

class Tooltip : public Component, public TooltipManager::Listener
{
public:
    explicit Tooltip(std::string defaultTooltip = "");
    ~Tooltip() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    
    void tooltipChanged(const std::string& newTooltip) override;

private:
    std::string _tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tooltip)
};

}
