#include "../../../include/gui/laf/TextInput.h"
#include "../../../include/gui/element/TextInput.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

TextInput::TextInput(const element::TextInput& parent):
    _parent(parent)
{
}

void TextInput::fillTextEditorBackground(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor)
{
    const auto backgroundBounds = textEditor.getLocalBounds().toFloat();
    g.setColour(_parent.getBackgroundColour());
    g.fillRoundedRectangle(backgroundBounds, _parent.getBorderRadius());
}

void TextInput::drawTextEditorOutline(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor)
{
    g.setColour(_parent.getBorderColour());
    g.drawRoundedRectangle(textEditor.getLocalBounds().toFloat(), _parent.getBorderRadius(), 1.f);
}

}
