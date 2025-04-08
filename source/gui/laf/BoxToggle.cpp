#include "../../../includes/gui/laf/BoxToggle.h"
#include "../../../includes/gui/Theme.h"
#include "../../../includes/gui/Helpers.h"
#include "../../../includes/gui/EmbeddedFonts.h"
#include "../../../includes/gui/element/BoxToggle.h"

namespace ui::laf
{

BoxToggle::BoxToggle()
{
}

BoxToggle::~BoxToggle()
{
}

void BoxToggle::drawTickBox
(
    juce::Graphics& g,
    juce::Component& button,
    float x,
    float y,
    float w,
    float h,
    bool ticked,
    bool isEnabled,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown
)
{
    auto whiteColor = ui::Theme::getInstance().getColor(ui::Theme::ThemeColor::EMPTY_SHADE).asJuce();
    auto disabledColor = ui::Theme::getInstance().getColor(ui::Theme::ThemeColor::DISABLED).asJuce();
    auto accentColor = ui::Theme::getInstance().getColor(ui::Theme::ThemeColor::ACCENT).asJuce();

    g.setColour(isEnabled ? whiteColor : disabledColor);
    
    if (shouldDrawButtonAsHighlighted)
    {
        auto highlightedColor = ui::Theme::getInstance().getColor(ui::Theme::ThemeColor::LIGHT_SHADE).asJuce();
        g.setColour(highlightedColor);
    }
    
    if (ticked)
    {
        if (isEnabled && static_cast<ui::element::BoxToggle&>(button).isActive())
        {
            g.setColour(accentColor);
        }
        g.drawRoundedRectangle(x, y, w, h, 4.0, 1.0);
        g.fillRoundedRectangle(x + 4, y + 4, w - 8, h - 8, 2.0);
    }
    else if (isEnabled && static_cast<ui::element::BoxToggle&>(button).isActive())
    {
        g.drawRoundedRectangle(x, y, w, h, 4.0, 1.0);
        g.setColour(accentColor.withAlpha(0.4f));
        g.fillRoundedRectangle(x + 4, y + 4, w - 8, h - 8, 2.0);
    }
    else
    {
        g.drawRoundedRectangle(x, y, w, h, 4.0, 1.0);
    }
}

}
