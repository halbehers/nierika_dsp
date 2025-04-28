#pragma once

#include "../Section.h"

namespace nierika::gui::layout
{

class AppLayout : public Section
{
public:
    AppLayout(dsp::ParameterManager& parameterManager, const std::string& pluginEnabledParameterID = "");
    ~AppLayout() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void displayTooltip();

    juce::Rectangle<int> getBypassButtonBounds() override;

private:
    element::Tooltip _tooltip;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppLayout)
};

}
