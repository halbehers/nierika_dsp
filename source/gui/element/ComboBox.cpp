#include "../../../includes/gui/element/ComboBox.h"
#include "../../../includes/gui/Theme.h"

namespace ui::element
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
