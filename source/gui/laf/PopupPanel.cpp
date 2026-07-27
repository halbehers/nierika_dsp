#include "../../../include/gui/laf/PopupPanel.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

void PopupPanel::drawCallOutBoxBackground(juce::CallOutBox& box, juce::Graphics& g, const juce::Path& path, juce::Image& cachedImage)
{
    (void) cachedImage;

    // Opt-in per-instance override (see PopupPanel::show/showNonModal callers) - defaults to fully
    // opaque so existing callers that never set this property are unaffected.
    const float backgroundOpacity = box.getProperties().getWithDefault("backgroundOpacity", 1.0f);

    g.setColour(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce().withAlpha(backgroundOpacity));
    g.fillPath(path);

    g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.2f));
    g.strokePath(path, juce::PathStrokeType(1.0f));
}

int PopupPanel::getCallOutBoxBorderSize(const juce::CallOutBox& box)
{
    (void) box;
    return 12;
}

float PopupPanel::getCallOutBoxCornerSize(const juce::CallOutBox& box)
{
    (void) box;
    return Theme::getBorderRadius();
}

}
