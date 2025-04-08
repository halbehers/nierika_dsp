#include "../../../includes/gui/element/BoxToggle.h"
#include "../../../includes/gui/Theme.h"
#include "../../../includes/gui/EmbeddedFonts.h"
#include "../../../includes/gui/Helpers.h"

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
