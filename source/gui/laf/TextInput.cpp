#include "../../../include/gui/laf/TextButton.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::laf
{

void TextInput::fillTextEditorBackground(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor)
{
    const juce::Colour whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    const juce::Colour grayColor = Theme::newColor(Theme::ThemeColor::LIGHTER_SHADE).asJuce();

    const auto backgroundBounds = textEditor.getLocalBounds().toFloat();
    g.setGradientFill(juce::ColourGradient(whiteColor.withAlpha(0.1f), textEditor.getWidth() / 2, 0.0, grayColor.withAlpha(0.1f), textEditor.getWidth() / 2, textEditor.getHeight(), false));
    g.fillRoundedRectangle(backgroundBounds, 17.f);
}

void TextInput::drawTextEditorOutline(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor)
{
    g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.2f));
    g.drawRoundedRectangle(textEditor.getLocalBounds().toFloat(), 17.f, 1.f);
}

}
