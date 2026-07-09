#pragma once

namespace nierika::gui::laf
{

class TextInput final : public juce::LookAndFeel_V4
{
public:
    TextInput() = default;
    ~TextInput() override = default;

    void fillTextEditorBackground(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor) override;
    void drawTextEditorOutline(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextInput)
};

}
