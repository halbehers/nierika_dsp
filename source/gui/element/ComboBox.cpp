#include "../../../include/gui/element/ComboBox.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

ComboBox::ComboBox(const juce::String& name)
{
    setLookAndFeel(&_lookAndFeel);
}

ComboBox::~ComboBox()
{
}

void ComboBox::paint (juce::Graphics& g)
{
    juce::ComboBox::paint(g);
}

void ComboBox::resized()
{
    juce::ComboBox::resized();
}

}
