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
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

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
    void setPasswordCharacter(juce::juce_wchar character) { _input.setPasswordCharacter(character); }
    void setCaretVisible (bool shouldBeVisible) { _input.setCaretVisible(shouldBeVisible); }
    bool isCaretVisible() const { return _input.isCaretVisible(); }
    void setIndents(int leftIndent, int rightIndent) { _input.setIndents(leftIndent, rightIndent); }
    int getTopIndent () const noexcept { return _input.getTopIndent(); }
    int getLeftIndent () const noexcept { return _input.getLeftIndent(); }

    void textEditorTextChanged(juce::TextEditor & editor) override;
    void textEditorReturnKeyPressed(juce::TextEditor & editor) override;

    void setHeightType(Theme::HeightType type) { _heightType = type; resetIndents(); resized(); }
    [[nodiscard]] Theme::HeightType getHeightType() const { return _heightType; }

    int getHeight() { return static_cast<int>(Theme::resolveHeight(_heightType, static_cast<float>(getLocalBounds().getHeight()))); }

    void setBorderRadius(float radius) { _borderRadiusOverride = radius; }
    void resetBorderRadius() { _borderRadiusOverride = -1; }
    float getBorderRadius() const { return _borderRadiusOverride >= 0 ? _borderRadiusOverride : Theme::getBorderRadius(); }

    void setBackgroundColour(juce::Colour colour) { _backgroundOverride = colour; }
    void resetBackgroundColour() { _backgroundOverride = juce::Colour(); }
    juce::Colour getBackgroundColour() const { return _backgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce()); }

    void setBorderColour(juce::Colour colour) { _borderOverride = colour; }
    void resetBorderColour() { _borderOverride = juce::Colour(); }
    juce::Colour getBorderColour() const { return _borderOverride.value_or(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.2f)); }

    void setRounded(bool isRounded) { setBorderRadius(getHeight() / 2); }


protected:
    juce::TextEditor _input {};

private:
    laf::TextInput _lookAndFeel { *this };

    std::vector<OnValueChangedListener*> _valueChangedListeners;
    std::vector<OnReturnKeyPressedListener*> _returnKeyListeners;

    
    std::optional<juce::Colour> _backgroundOverride = std::nullopt;
    std::optional<juce::Colour> _borderOverride = std::nullopt;
    std::string _placeholderText;


    float _borderRadiusOverride = -1;
    Theme::HeightType _heightType = Theme::HeightType::AUTO;

    void setup();
    void resetIndents();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextInput)
};

}
 
