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

    void setBorderRadius(float radius) { _borderRadiusOverride = radius; }
    void resetBorderRadius() { _borderRadiusOverride = -1; }
    float getBorderRadius() const { return _borderRadiusOverride >= 0 ? _borderRadiusOverride : Theme::getBorderRadius(); }

    void setBackgroundColour(juce::Colour colour) { _backgroundOverride = colour; }
    void resetBackgroundColour() { _backgroundOverride = juce::Colour(); }
    juce::Colour getBackgroundColour() const { return _backgroundOverride.isTransparent() ? Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce() : _backgroundOverride; }

    void setBorderColour(juce::Colour colour) { _borderOverride = colour; }
    void resetBorderColour() { _borderOverride = juce::Colour(); }
    juce::Colour getBorderColour() const { return _borderOverride.isTransparent() ? Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.2f) : _borderOverride; }

protected:
    juce::TextEditor _input {};

private:
    laf::TextInput _lookAndFeel { *this };

    std::vector<OnValueChangedListener*> _valueChangedListeners;
    std::vector<OnReturnKeyPressedListener*> _returnKeyListeners;

    juce::Colour _backgroundOverride = juce::Colour();
    juce::Colour _borderOverride = juce::Colour();


    float _borderRadiusOverride = -1;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextInput)
};

}
 
