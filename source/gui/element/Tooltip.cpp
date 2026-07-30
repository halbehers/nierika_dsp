#include <utility>

#include "../../../include/gui/element/Tooltip.h"
#include "../../../include/gui/TooltipManager.h"

namespace nierika::gui::element
{

Tooltip::Tooltip(std::string defaultTooltip):
    Component("tooltip"),
    _tooltip(std::move(defaultTooltip))
{
    TooltipManager::getInstance().setDefaultTooltip(_tooltip);
    TooltipManager::getInstance().addListener(this);
}

Tooltip::~Tooltip()
{
    if (TooltipManager::isAlive())
        TooltipManager::getInstance().removeListener(this);
}

void Tooltip::tooltipChanged(const std::string& newTooltip)
{
    _tooltip = newTooltip;
    repaint();
}

void Tooltip::paint (juce::Graphics& g)
{
    const auto whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE);

    g.setFont(Theme::newFont(Theme::LIGHT).withHeight(static_cast<float>(getHeight() - getHeight() / 4)));
    g.setColour(whiteColor.asJuce());

    g.drawFittedText(_tooltip, 28, 0, getWidth(), getHeight(), juce::Justification::left, 1);

    const auto grayColor = Theme::newColor(Theme::ThemeColor::LIGHTER_SHADE);
    g.setGradientFill(juce::ColourGradient(whiteColor.asJuce().withAlpha(0.05f), static_cast<float>(getWidth() / 2), 0.0f, grayColor.asJuce().withAlpha(0.05f), static_cast<float>(getWidth() / 2), static_cast<float>(getHeight()), false));
    g.fillRoundedRectangle(0.f, 0.f, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 8.f);

    if (!_tooltip.empty())
    {
        constexpr int iconSize = 8;
        helpers::drawFromSVG(g, Icons::getInfo(), whiteColor.asHexString(), 12, getHeight() / 2 - iconSize / 2 + 1, iconSize, iconSize, juce::AffineTransform());
    }
    
}

void Tooltip::resized()
{
    Component::resized();
}

}
