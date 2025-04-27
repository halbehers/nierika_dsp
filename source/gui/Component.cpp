#include "../../include/gui/Component.h"
#include "../../include/gui/TooltipManager.h"

namespace nierika::gui
{

Component::Component(const std::string& identifier, const std::string& name, const std::string& tooltip):
    juce::Component()
{
    setComponentID(identifier);
    setName(name);

    _margin.attachComponent(this);
    _margin.setAsMargin();

    setTooltip(tooltip);
}

Component::~Component()
{
    TooltipManager::getInstance().unregisterComponent(*this);
}

void Component::paint(juce::Graphics& g)
{
    if (_background.display)
    {
        const auto backgroundBounds = getOuterLocalBounds().toFloat();
        
        if (_background.isGradient)
        {
            g.setGradientFill(_background.gradient);
            g.fillRoundedRectangle(backgroundBounds, _background.radius);
        }
        else
        {
            g.setColour(_background.color);
            g.fillRoundedRectangle(backgroundBounds, _background.radius);
        }
    }
    if (_border.display)
    {
        g.setColour(_border.color);
        g.drawRoundedRectangle(getOuterLocalBounds().toFloat(), _border.radius, _border.lineThickness);
    }
}

juce::Rectangle<int> Component::getOuterLocalBounds()
{
    return _margin.computeBounds().toNearestInt();
}

juce::Rectangle<int> Component::getLocalBounds()
{
    return _padding.computeBounds(getOuterLocalBounds().toFloat()).toNearestInt();
}

void Component::setTooltip(const std::string& text)
{
    _tooltip = text;
    if (!_tooltipEnabled && text != "") setTooltipEnabled(true);
}

const std::string& Component::getTooltip() const
{
    return _tooltip;
}

void Component::setTooltipEnabled(bool enabled)
{
    _tooltipEnabled = enabled;
    if (enabled)
        TooltipManager::getInstance().registerComponent(*this);
    else
        TooltipManager::getInstance().unregisterComponent(*this);
}

bool Component::isTooltipEnabled() const
{
    return _tooltipEnabled;
}

void Component::displayBorder(Theme::ThemeColor color, float lineThickness, float radius, float alpha)
{
    juce::Colour c = Theme::getInstance().getColor(color).asJuce();
    c = c.withAlpha(alpha);
    displayBorder(c, lineThickness, radius);
}

void Component::displayBorder(juce::Colour color, float lineThickness, float radius)
{
    _border.display = true;
    _border.color = color;
    _border.lineThickness = lineThickness;
    _border.radius = radius;
}

void Component::hideBorder()
{
    _border.display = false;
}

void Component::displayBackground(Theme::ThemeColor color, float radius, float alpha)
{
    juce::Colour c = Theme::getInstance().getColor(color).asJuce();
    c = c.withAlpha(alpha);
    displayBackground(c, radius);
}

void Component::displayBackground(juce::Colour color, float radius)
{
    _background.display = true;
    _background.color = color;
    _background.radius = radius;
    _background.isGradient = false;
}

void Component::displayBackground(juce::ColourGradient gradient, float radius)
{
    _background.display = true;
    _background.gradient = gradient;
    _background.radius = radius;
    _background.isGradient = true;
}

void Component::hideBackground()
{
    _background.display = false;
}

template<typename T>
void Component::setMargin(layout::Spacing<T> margin)
{
    _margin = margin.toFloat();
}

template<typename T>
void Component::setMargin(const T marginLeft, const T marginTop, const T marginRight, const T marginBottom)
{
    _margin = layout::Spacing<T>(marginLeft, marginTop, marginRight, marginBottom).toFloat();
}

template<typename T>
void Component::setMargin(const T horizontalMargin, const T verticalMargin)
{
    _margin = layout::Spacing<T>(horizontalMargin, verticalMargin).toFloat();
}

template<typename T>
void Component::setMargin(const T value)
{
    _margin = layout::Spacing<T>(value).toFloat();
}

template<typename T>
void Component::setPadding(layout::Spacing<T> padding)
{
    _padding = padding.toFloat();
}

template<typename T>
void Component::setPadding(const T paddingLeft, const T paddingTop, const T paddingRight, const T paddingBottom)
{
    _padding = layout::Spacing<T>(paddingLeft, paddingTop, paddingRight, paddingBottom).toFloat();
}

template<typename T>
void Component::setPadding(const T horizontalPadding, const T verticalPadding)
{
    _padding = layout::Spacing<T>(horizontalPadding, verticalPadding).toFloat();
}

template<typename T>
void Component::setPadding(const T value)
{
    _padding = layout::Spacing<T>(value).toFloat();
}

template void Component::setMargin(layout::Spacing<int> margin);
template void Component::setMargin(layout::Spacing<float> margin);
template void Component::setMargin(layout::Spacing<double> margin);
template void Component::setMargin(const int marginLeft, const int marginTop, const int marginRight, const int marginBottom);
template void Component::setMargin(const float marginLeft, const float marginTop, const float marginRight, const float marginBottom);
template void Component::setMargin(const double marginLeft, const double marginTop, const double marginRight, const double marginBottom);
template void Component::setMargin(const int horizontalMargin, const int verticalMargin);
template void Component::setMargin(const float horizontalMargin, const float verticalMargin);
template void Component::setMargin(const double horizontalMargin, const double verticalMargin);
template void Component::setMargin(const int value);
template void Component::setMargin(const float value);
template void Component::setMargin(const double value);

template void Component::setPadding(layout::Spacing<int> padding);
template void Component::setPadding(layout::Spacing<float> padding);
template void Component::setPadding(layout::Spacing<double> padding);
template void Component::setPadding(const int paddingLeft, const int paddingTop, const int paddingRight, const int paddingBottom);
template void Component::setPadding(const float paddingLeft, const float paddingTop, const float paddingRight, const float paddingBottom);
template void Component::setPadding(const double paddingLeft, const double paddingTop, const double paddingRight, const double paddingBottom);
template void Component::setPadding(const int horizontalPadding, const int verticalPadding);
template void Component::setPadding(const float horizontalPadding, const float verticalPadding);
template void Component::setPadding(const double horizontalPadding, const double verticalPadding);
template void Component::setPadding(const int value);
template void Component::setPadding(const float value);
template void Component::setPadding(const double value);

}
