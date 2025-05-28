#include "../../../include/gui/Helpers.h"
#include "../../../include/gui/element/IncrementalButton.h"
#include "../../../include/gui/Icons.h"

namespace nierika::gui::element
{

IncrementalButton::IncrementalButton(const juce::String& buttonName, Type type):
    juce::ArrowButton(buttonName, 0.0f, Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce()),
    _type(type),
    _svgBinary(type == Type::PLUS ? Icons::getPlus() : Icons::getMinus())
{
    
}


IncrementalButton::IncrementalButton(Type type):
    juce::ArrowButton(type == Type::PLUS ? "Plus" : "Minus", 0.0f, Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce()),
    _type(type),
    _svgBinary(type == Type::PLUS ? Icons::getPlus() : Icons::getMinus())
{
    
}

IncrementalButton::~IncrementalButton()
{
}

void IncrementalButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    const std::string whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asHexString();
    const std::string highlightedColor = Theme::getInstance().getColor(Theme::ThemeColor::LIGHT_SHADE).asHexString();
    const std::string downColor = Theme::getInstance().getColor(Theme::ThemeColor::MEDIUM_SHADE).asHexString();
    const std::string disabledColor = Theme::getInstance().getColor(Theme::ThemeColor::DISABLED).asHexString();
    
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

    constexpr int padding = 16;

    helpers::drawFromSVG(g, _svgBinary, color, padding, padding, size - padding * 2, size - padding * 2, juce::AffineTransform());
}

}
