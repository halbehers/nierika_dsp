#include "../../../include/gui/Helpers.h"
#include "../../../include/gui/element/IncrementalButton.h"
#include "../../../include/gui/Icons.h"

namespace nierika::gui::element
{

IncrementalButton::IncrementalButton(const juce::String& buttonName, Type type):
    juce::ArrowButton(buttonName, 0.0f, Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce()),
    _type(type),
    _svgBinary(type == Type::PLUS ? Icons::getPlus() : Icons::getMinus())
{
    
}


IncrementalButton::IncrementalButton(Type type):
    juce::ArrowButton(type == Type::PLUS ? "Plus" : "Minus", 0.0f, Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce()),
    _type(type),
    _svgBinary(type == Type::PLUS ? Icons::getPlus() : Icons::getMinus())
{
    
}

IncrementalButton::~IncrementalButton()
{
}

void IncrementalButton::setPadding(int padding)
{
    _padding = padding;
}

int IncrementalButton::getPadding() const
{
    return _padding;
}

void IncrementalButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    const std::string whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asHexString();
    const std::string highlightedColor = Theme::newColor(Theme::ThemeColor::LIGHT_SHADE).asHexString();
    const std::string downColor = Theme::newColor(Theme::ThemeColor::MEDIUM_SHADE).asHexString();
    const std::string disabledColor = Theme::newColor(Theme::ThemeColor::DISABLED).asHexString();
    
    auto color = whiteColor;
    
    if (!isEnabled())
    {
        color = disabledColor;
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        color = highlightedColor;
    }
    else if (shouldDrawButtonAsDown)
    {
        color = downColor;
    }

    const int size = std::min(getWidth(), getHeight());

    const int baseX = std::max(0, getWidth() - size) / 2;
    const int baseY = std::max(0, getHeight() - size) / 2;

    helpers::drawFromSVG(g, _svgBinary, color, baseX + getPadding(), baseY + getPadding(), size - getPadding() * 2, size - getPadding() * 2, juce::AffineTransform());
}

}
