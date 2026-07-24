#include "../../include/gui/Component.h"
#include "../../include/gui/TooltipManager.h"

namespace nierika::gui
{

Component::Component(const std::string& identifier, const std::string& name, const std::string& tooltip)
{
    setComponentID(identifier);
    setName(name);

    _margin.attachComponent(this);
    _margin.setAsMargin();

    setTooltip(tooltip);

    Theme::getChangeBroadcaster().addChangeListener(this);
}

Component::~Component()
{
    Theme::getChangeBroadcaster().removeChangeListener(this);
    TooltipManager::getInstance().unregisterComponent(*this);
}

void Component::changeListenerCallback(juce::ChangeBroadcaster*)
{
    if (_border.themeColor.has_value())
        _border.color = Theme::newColor(*_border.themeColor).asJuce().withAlpha(_border.alpha);
    if (_background.themeColor.has_value() && !_background.isGradient)
        _background.color = Theme::newColor(*_background.themeColor).asJuce().withAlpha(_background.alpha);

    repaint();
}

void Component::resized()
{
    if (_fixedHeight >= 0.f)
    {
        // Extra space is what's left after _margin's own top/bottom inset is honoured, not the
        // raw component height - otherwise _margin and _fixedSizedMargin would both apply on top
        // of each other and the visible box would end up smaller than _fixedHeight by however
        // much vertical margin is set.
        const auto availableHeight = static_cast<float>(getHeight()) - _margin.top - _margin.bottom;
        const auto extra = juce::jmax(0.f, availableHeight - _fixedHeight);

        switch (_verticalAlignment)
        {
            case START:  _fixedSizedMargin.top = 0.f;         _fixedSizedMargin.bottom = extra;        break;
            case END:    _fixedSizedMargin.top = extra;       _fixedSizedMargin.bottom = 0.f;          break;
            case CENTER: _fixedSizedMargin.top = extra / 2.f; _fixedSizedMargin.bottom = extra / 2.f;  break;
        }
    }

    if (_fixedWidth >= 0.f)
    {
        const auto availableWidth = static_cast<float>(getWidth()) - _margin.left - _margin.right;
        const auto extra = juce::jmax(0.f, availableWidth - _fixedWidth);

        switch (_horizontalAlignment)
        {
            case START:  _fixedSizedMargin.left = 0.f;         _fixedSizedMargin.right = extra;         break;
            case END:    _fixedSizedMargin.left = extra;       _fixedSizedMargin.right = 0.f;           break;
            case CENTER: _fixedSizedMargin.left = extra / 2.f; _fixedSizedMargin.right = extra / 2.f;   break;
        }
    }
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
    const auto finalMargin = _margin + _fixedSizedMargin;

    // Not finalMargin.computeBounds() - that overload resolves bounds via the Spacing's own
    // attached _component pointer, which Spacing's copy constructor doesn't carry over (see
    // operator+), so a freshly-computed Spacing like this one is never attached to anything and
    // that overload would always return an empty rect. Pass this component explicitly instead.
    return finalMargin.computeBounds(*this).toNearestInt();
}

int Component::getOuterX()
{
    return getOuterLocalBounds().getX();
}

int Component::getOuterY()
{
    return getOuterLocalBounds().getY();
}

int Component::getOuterWidth()
{
    return getOuterLocalBounds().getWidth();
}

int Component::getOuterHeight()
{
    return getOuterLocalBounds().getHeight();
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

    if (!TooltipManager::isAlive()) return;

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
    displayBorder(Theme::newColor(color).asJuce().withAlpha(alpha), lineThickness, radius);
    _border.themeColor = color;
    _border.alpha = alpha;
}

void Component::displayBorder(juce::Colour color, float lineThickness, float radius)
{
    _border.display = true;
    _border.color = color;
    _border.lineThickness = lineThickness;
    _border.radius = radius;
    _border.themeColor.reset();
}

void Component::hideBorder()
{
    _border.display = false;
}

void Component::displayBackground(Theme::ThemeColor color, float radius, float alpha)
{
    displayBackground(Theme::newColor(color).asJuce().withAlpha(alpha), radius);
    _background.themeColor = color;
    _background.alpha = alpha;
}

void Component::displayBackground(juce::Colour color, float radius)
{
    _background.display = true;
    _background.color = color;
    _background.radius = radius;
    _background.isGradient = false;
    _background.themeColor.reset();
}

void Component::displayBackground(juce::ColourGradient gradient, float radius)
{
    _background.display = true;
    _background.gradient = gradient;
    _background.radius = radius;
    _background.isGradient = true;
    _background.themeColor.reset();
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
void Component::withTopMargin(const T value)
{
    _margin.withTop(static_cast<float>(value));
}

template<typename T>
void Component::withLeftMargin(const T value)
{
    _margin.withLeft(static_cast<float>(value));
}

template<typename T>
void Component::withRightMargin(const T value)
{
    _margin.withRight(static_cast<float>(value));
}

template<typename T>
void Component::withBottomMargin(const T value)
{
    _margin.withBottom(static_cast<float>(value));
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

template<typename T>
void Component::withTopPadding(const T value)
{
    _padding.withTop(static_cast<float>(value));
}

template<typename T>
void Component::withLeftPadding(const T value)
{
    _padding.withLeft(static_cast<float>(value));
}

template<typename T>
void Component::withRightPadding(const T value)
{
    _padding.withRight(static_cast<float>(value));
}

template<typename T>
void Component::withBottomPadding(const T value)
{
    _padding.withBottom(static_cast<float>(value));
}

template void Component::setMargin(layout::Spacing<int> margin);
template void Component::setMargin(layout::Spacing<float> margin);
template void Component::setMargin(layout::Spacing<double> margin);
template void Component::setMargin(int marginLeft, int marginTop, int marginRight, int marginBottom);
template void Component::setMargin(float marginLeft, float marginTop, float marginRight, float marginBottom);
template void Component::setMargin(double marginLeft, double marginTop, double marginRight, double marginBottom);
template void Component::setMargin(int horizontalMargin, int verticalMargin);
template void Component::setMargin(float horizontalMargin, float verticalMargin);
template void Component::setMargin(double horizontalMargin, double verticalMargin);
template void Component::setMargin(int value);
template void Component::setMargin(float value);
template void Component::setMargin(double value);
template void Component::withTopMargin(int value);
template void Component::withTopMargin(float value);
template void Component::withTopMargin(double value);
template void Component::withLeftMargin(int value);
template void Component::withLeftMargin(float value);
template void Component::withLeftMargin(double value);
template void Component::withRightMargin(int value);
template void Component::withRightMargin(float value);
template void Component::withRightMargin(double value);
template void Component::withBottomMargin(int value);
template void Component::withBottomMargin(float value);
template void Component::withBottomMargin(double value);

template void Component::setPadding(layout::Spacing<int> padding);
template void Component::setPadding(layout::Spacing<float> padding);
template void Component::setPadding(layout::Spacing<double> padding);
template void Component::setPadding(int paddingLeft, int paddingTop, int paddingRight, int paddingBottom);
template void Component::setPadding(float paddingLeft, float paddingTop, float paddingRight, float paddingBottom);
template void Component::setPadding(double paddingLeft, double paddingTop, double paddingRight, double paddingBottom);
template void Component::setPadding(int horizontalPadding, int verticalPadding);
template void Component::setPadding(float horizontalPadding, float verticalPadding);
template void Component::setPadding(double horizontalPadding, double verticalPadding);
template void Component::setPadding(int value);
template void Component::setPadding(float value);
template void Component::setPadding(double value);
template void Component::withTopPadding(int value);
template void Component::withTopPadding(float value);
template void Component::withTopPadding(double value);
template void Component::withLeftPadding(int value);
template void Component::withLeftPadding(float value);
template void Component::withLeftPadding(double value);
template void Component::withRightPadding(int value);
template void Component::withRightPadding(float value);
template void Component::withRightPadding(double value);
template void Component::withBottomPadding(int value);
template void Component::withBottomPadding(float value);
template void Component::withBottomPadding(double value);

}
