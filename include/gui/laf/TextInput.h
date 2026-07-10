#pragma once

namespace nierika::gui::element { class TextInput; }

namespace nierika::gui::laf
{

class TextInput final : public juce::LookAndFeel_V4
{
public:
    explicit TextInput(const element::TextInput& parent);
    ~TextInput() override = default;

    void fillTextEditorBackground(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor) override;
    void drawTextEditorOutline(juce::Graphics& g, int width, int	height, juce::TextEditor& textEditor) override;

private:
    const element::TextInput& _parent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextInput)
};

}
