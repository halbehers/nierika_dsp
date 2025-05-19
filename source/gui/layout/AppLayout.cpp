#include "../../../include/gui/layout/AppLayout.h"

namespace nierika::gui::layout
{

AppLayout::AppLayout(dsp::ParameterManager& parameterManager, const std::string& pluginEnabledParameterID):
    Section("app-layout", parameterManager, pluginEnabledParameterID)
{
    addChildComponent(_tooltip);
    if (!pluginEnabledParameterID.empty())
        setBypassable(true);
}

void AppLayout::displayTooltip()
{
    _tooltip.setVisible(true);
}

void AppLayout::paint(juce::Graphics& g)
{
    Section::paint(g);
}

void AppLayout::resized()
{
    Section::resized();
    _tooltip.setBounds(getLayout().getRectangleAtBottom(20.f, 16.f).toNearestInt());
}

juce::Rectangle<int> AppLayout::getBypassButtonBounds()
{
    int size = 24;
    return juce::Rectangle<int>(getWidth() - 16 - size, 16, size + 1, size);
}

}
