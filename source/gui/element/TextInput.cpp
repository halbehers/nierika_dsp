#include "../../../include/gui/element/TextInput.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

TextInput::TextInput(const std::string& identifier):
    Component(identifier)
{
    setup();
}

void TextInput::setup()
{
    addAndMakeVisible(_input);

    _input.setLookAndFeel(&_lookAndFeel);

    setMultiline(false);
    setCaretVisible(true);

    setIndents(24, 10);

    _input.addListener(this);
}

void TextInput::textEditorTextChanged(juce::TextEditor & editor)
{
    for (auto listener : _valueChangedListeners)
        listener->onValueChanged(getID(), editor.getText().toStdString());
}

void TextInput::textEditorReturnKeyPressed(juce::TextEditor & editor)
{
    for (auto listener : _returnKeyListeners)
        listener->onReturnKeyPressed(getID(), editor.getText().toStdString());
}

void TextInput::resized()
{
    Component::resized();
    _input.setBounds(getLocalBounds());
    _input.setFont(Theme::newFont(Theme::LIGHT).withHeight(getHeight() * 0.55f));
}

void TextInput::setHelpText(const std::string& helpText)
{
    return _input.setHelpText(helpText);
}

void TextInput::setText(const std::string& text, bool sendTextChangeMessage)
{
    _input.setText(text, sendTextChangeMessage);
}

void TextInput::addOnValueChangedListener(OnValueChangedListener* listener)
{
    _valueChangedListeners.push_back(listener);
}

void TextInput::removeOnValueChangedListener(OnValueChangedListener* listener)
{
    _valueChangedListeners.erase(
        std::remove(_valueChangedListeners.begin(), _valueChangedListeners.end(), listener),
        _valueChangedListeners.end()
    );
}

void TextInput::addOnReturnKeyPressedListener(OnReturnKeyPressedListener* listener)
{
    _returnKeyListeners.push_back(listener);
}

void TextInput::removeReturnKeyPressedListener(OnReturnKeyPressedListener* listener)
{
    _returnKeyListeners.erase(
        std::remove(_returnKeyListeners.begin(), _returnKeyListeners.end(), listener),
        _returnKeyListeners.end()
    );
}

void TextInput::setPlaceholder(const std::string& text)
{
    _input.setTextToShowWhenEmpty(juce::String(text), Theme::newColor(Theme::ThemeColor::TEXT).asJuce().withAlpha(0.5f));
}

}
