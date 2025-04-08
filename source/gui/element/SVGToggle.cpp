#include "../../../include/gui/element/SVGToggle.h"
#include "../../../include/gui/Theme.h"
#include "../../../include/gui/EmbeddedFonts.h"
#include "../../../include/gui/Helpers.h"

namespace ui::element
{

SVGToggle::SVGToggle(const char* svgBinary):
    juce::ToggleButton(),
    _lookAndFeel(svgBinary)
{
    init();
}

SVGToggle::SVGToggle(const char* svgBinary, int width, int height):
    juce::ToggleButton(),
    _lookAndFeel(svgBinary),
    _width(width),
    _height(height)
{
    init();
}


SVGToggle::~SVGToggle()
{
}

void SVGToggle::init()
{
    setLookAndFeel(&_lookAndFeel);
    setToggleState(true, juce::NotificationType::dontSendNotification);
    
    if (_width != -1 && _height != -1) setSize(_width, _height);
}

void SVGToggle::paint (juce::Graphics& g)
{
    juce::ToggleButton::paint(g);
}

void SVGToggle::resized()
{
    juce::ToggleButton::resized();
    if (_width != -1 && _height != -1)
    {
        setBounds(getX(), getY(), _width, _height);
    }
}

}
