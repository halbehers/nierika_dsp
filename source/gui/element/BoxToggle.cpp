#include "../../../include/gui/element/BoxToggle.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/EmbeddedFonts.h"
#include "../../../include/gui/Helpers.h"

namespace ui::element
{

BoxToggle::BoxToggle():
    juce::ToggleButton()
{
    setLookAndFeel(&_lookAndFeel);
    setToggleState(true, juce::NotificationType::dontSendNotification);
}


BoxToggle::~BoxToggle()
{
}

void BoxToggle::setActive(bool isActive)
{
    _isActive = isActive;
    repaint();
}

bool BoxToggle::isActive()
{
    return _isActive;
}

void BoxToggle::paint(juce::Graphics& g)
{
    juce::ToggleButton::paint(g);
}

void BoxToggle::resized()
{
    juce::ToggleButton::resized();
}

}
