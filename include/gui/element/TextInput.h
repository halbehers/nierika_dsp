#pragma once

#include "../Component.h"
#include "../laf/TextInput.h"

namespace nierika::gui::element
{

class TextInput: public Component, public juce::TextEditor::Listener
{
public:    
    struct OnValueChangedListener
    {
        virtual ~OnValueChangedListener() = default;
        virtual void onValueChanged(const std::string& componentID, const std::string& newValue) = 0;
    };

    struct OnReturnKeyPressedListener
    {
        virtual ~OnReturnKeyPressedListener() = default;
        virtual void onReturnKeyPressed(const std::string& componentID, const std::string& newValue) = 0;
    };

    TextInput(const std::string& identifier);
    ~TextInput() override = default;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeOnValueChangedListener(OnValueChangedListener* listener);
    void addOnReturnKeyPressedListener(OnReturnKeyPressedListener* listener);
    void removeReturnKeyPressedListener(OnReturnKeyPressedListener* listener);

    void setPlaceholder(const std::string& text);
    void setHelpText(const std::string& helpText);

    std::string getText() const { return _input.getText().toStdString(); }
    void setText(const std::string& text, bool sendTextChangeMessage=true);
    void setReadOnly(bool isReadOnly) { _input.setReadOnly(isReadOnly); }

    void setMultiline(bool isMultiline) { _input.setMultiLine(isMultiline); }
    bool isMultiline() const { return _input.isMultiLine(); }
    void setCaretVisible (bool shouldBeVisible) { _input.setCaretVisible(shouldBeVisible); }
    bool isCaretVisible() const { return _input.isCaretVisible(); }
    void setIndents(int leftIndent, int rightIndent) { _input.setIndents(leftIndent, rightIndent); }
    int getTopIndent () const noexcept { return _input.getTopIndent(); }
    int getLeftIndent () const noexcept { return _input.getLeftIndent(); }

    void textEditorTextChanged(juce::TextEditor & editor) override;
    void textEditorReturnKeyPressed(juce::TextEditor & editor) override;

protected:
    juce::TextEditor _input {};

private:
    laf::TextInput _lookAndFeel;

    std::vector<OnValueChangedListener*> _valueChangedListeners;
    std::vector<OnReturnKeyPressedListener*> _returnKeyListeners;

    std::string _placeholder;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextInput)
};

}
 
