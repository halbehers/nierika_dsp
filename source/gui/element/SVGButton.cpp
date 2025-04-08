#include "../../../includes/gui/element/SVGButton.h"
#include "../../../includes/gui/Theme.h"
#include "../../../includes/gui/EmbeddedFonts.h"
#include "../../../includes/gui/Helpers.h"

namespace ui::element
{

SVGButton::SVGButton(const char* svgBinary):
juce::DrawableButton("button", ButtonStyle::ImageFitted),
    _lookAndFeel(svgBinary)
{
    init();
}

SVGButton::SVGButton(const char* svgBinary, int width, int height):
    juce::DrawableButton("button", ButtonStyle::ImageFitted),
    _lookAndFeel(svgBinary),
    _width(width),
    _height(height)
{
    init();
}


SVGButton::~SVGButton()
{
}

void SVGButton::init()
{
    setLookAndFeel(&_lookAndFeel);
}

void SVGButton::paint (juce::Graphics& g)
{
    juce::DrawableButton::paint(g);
}

void SVGButton::resized()
{
    juce::DrawableButton::resized();
    if (_width != -1 && _height != -1)
    {
        setSize(_width, _height);
    }
}

}
