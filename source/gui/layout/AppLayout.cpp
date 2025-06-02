#include "../../../include/gui/layout/AppLayout.h"

namespace nierika::gui::layout
{

AppLayout::AppLayout(dsp::ParameterManager& parameterManager, const std::string& pluginEnabledParameterID):
    Section("app-layout", parameterManager, pluginEnabledParameterID),
    _windowManager(*this)
{
    addChildComponent(_tooltip);
    if (!pluginEnabledParameterID.empty())
        setBypassable(true);

    _windowManager.createWindow(std::make_unique<Dialog>("dialog", _windowManager, "Ceci est une info qui ne sert a rien, enjoy la vida !"), Distance(70, Distance<>::PERCENTAGE, 400), Distance(20, Distance<>::PERCENTAGE, 40));
    _windowManager.showWindow("dialog");
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
    _windowManager.setBounds(getLocalBounds());
    _tooltip.setBounds(getLayout().getRectangleAtBottom(20.f, 16.f).toNearestInt());
}

juce::Rectangle<int> AppLayout::getBypassButtonBounds()
{
    constexpr int size = 24;
    return juce::Rectangle<int>(getWidth() - 16 - size, 16, size + 1, size);
}

}
