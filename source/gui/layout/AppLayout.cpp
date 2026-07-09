#include "../../../include/gui/layout/AppLayout.h"

namespace nierika::gui::layout
{

AppLayout::AppLayout(dsp::ParameterManager& parameterManager, const std::string& pluginEnabledParameterID):
    Section("app-layout", parameterManager, pluginEnabledParameterID)
{
    addChildComponent(_tooltip);
    if (!pluginEnabledParameterID.empty())
        setBypassable(true);

    // auto dialog = std::make_unique<Dialog>("dialog", _windowManager);
    // dialog->setTitle("Dialog title");
    // dialog->setText("Ceci est une info qui ne sert a rien, enjoy la vida !");
    // _windowManager.createWindow(std::move(dialog));
    // _windowManager.showWindow("dialog");
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
    // _windowManager.setBounds(getLocalBounds());

    _tooltip.setBounds(getLayout().getRectangleAtBottom(20.f, 16.f).toNearestInt());
}

juce::Rectangle<int> AppLayout::getBypassButtonBounds()
{
    constexpr int size = 24;
    return juce::Rectangle<int>(getWidth() - 16 - size, 16, size + 1, size);
}

}
