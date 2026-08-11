#pragma once

#include <memory>
#include <optional>
#include <string>

#include "../Component.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

class ValueSetter: public Component, private juce::TextEditor::Listener
{
public:
    // Stacked (default): label above, value pill below - two rows. Inline: both share one row,
    // the label right-justified immediately left of the value pill.
    enum class DisplayMode { Stacked, Inline };

    // decimalPlaces defaults to 1, not 0: juce::String(float, 0) doesn't mean "no decimals" - with
    // 0 it falls back to the C++ stream's default double formatting (variable precision, switches
    // to scientific notation near zero, e.g. "-2.23517e-08"), which is never what a value pill
    // display wants. Pass a higher value explicitly for finer-grained parameters.
    ValueSetter(const std::string& identifier, const std::string& label, float minValue, float maxValue,
                float defaultValue, const std::string& valueSuffix = "", int decimalPlaces = 1);
    ValueSetter(dsp::ParameterManager& parameterManager, const std::string& parameterID,
                const std::string& valueSuffix = "", int decimalPlaces = 1);
    ~ValueSetter() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;

    void setDisplayLabel(const juce::String& label);
    void setGap(int gap) { _gap = gap; resized(); }

    void setDisplayMode(DisplayMode mode) { _displayMode = mode; resized(); repaint(); }
    [[nodiscard]] DisplayMode getDisplayMode() const { return _displayMode; }

    [[nodiscard]] float getValue() const { return _currentValue; }

    void setFontSize(Theme::FontSize size);
    void resetFontSize();
    [[nodiscard]] Theme::FontSize getFontSize() const { return _fontSizeOverride.value_or(Theme::PARAGRAPH); }

    // Mirrors ComboBox/Cycler's own setBackgroundColour convention. getValueBackgroundColour() is
    // called fresh inside paint() every repaint, and re-resolves _valueBackgroundThemeColorOverride
    // live rather than caching a resolved Colour - so unlike Dial/VerticalSlider this needs no
    // changeListenerCallback of its own; the base class's inherited repaint() on any theme
    // notification is all that's required for it to pick up a live change.
    void setValueBackgroundColour(juce::Colour colour) { _valueBackgroundThemeColorOverride.reset(); _valueBackgroundOverride = colour; repaint(); }
    void setValueBackgroundColour(Theme::ThemeColor colorId) { _valueBackgroundThemeColorOverride = colorId; repaint(); }
    void resetValueBackgroundColour() { _valueBackgroundThemeColorOverride.reset(); _valueBackgroundOverride = juce::Colour(); repaint(); }
    [[nodiscard]] juce::Colour getValueBackgroundColour() const
    {
        if (_valueBackgroundThemeColorOverride) return Theme::newColor(*_valueBackgroundThemeColorOverride).asJuce();
        return _valueBackgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce());
    }

    // When set, the value background (and the value text/edit box drawn on top of it) shrinks to
    // this size instead of filling the whole pill area, and setValueBackground{Horizontal,Vertical}
    // Alignment - Component's own Alignment enum (START/END/CENTER) - positions it within the
    // leftover space, same "extra space distributed by alignment" idea as Component's own
    // setFixedWidth/setFixedHeight + setHorizontal/VerticalAlignment.
    void setValueBackgroundWidth(float width) { _valueBackgroundWidthOverride = width; resized(); repaint(); }
    void resetValueBackgroundWidth() { _valueBackgroundWidthOverride = std::nullopt; resized(); repaint(); }
    [[nodiscard]] float getValueBackgroundWidth() { return _valueBackgroundWidthOverride.value_or(static_cast<float>(getPillBounds().getWidth())); }

    void setValueBackgroundHeight(float height) { _valueBackgroundHeightOverride = height; resized(); repaint(); }
    void resetValueBackgroundHeight() { _valueBackgroundHeightOverride = std::nullopt; resized(); repaint(); }
    [[nodiscard]] float getValueBackgroundHeight() { return _valueBackgroundHeightOverride.value_or(static_cast<float>(getPillBounds().getHeight())); }

    void setValueBackgroundHorizontalAlignment(Alignment alignment) { _valueBackgroundHorizontalAlignment = alignment; resized(); repaint(); }
    [[nodiscard]] Alignment getValueBackgroundHorizontalAlignment() const { return _valueBackgroundHorizontalAlignment; }

    void setValueBackgroundVerticalAlignment(Alignment alignment) { _valueBackgroundVerticalAlignment = alignment; resized(); repaint(); }
    [[nodiscard]] Alignment getValueBackgroundVerticalAlignment() const { return _valueBackgroundVerticalAlignment; }

private:
    static constexpr float kLabelHeight = 16.0f;
    static constexpr float kDragPixelsForFullRange = 150.0f; // horizontal pixels dragged to sweep min..max

    void setup();
    void applyValue(float newValue, bool asCompleteGesture);
    void beginTextEdit();
    void commitTextEdit();
    void cancelTextEdit();
    [[nodiscard]] juce::String formatValue(float value) const;
    [[nodiscard]] juce::Rectangle<int> getPillBounds();
    [[nodiscard]] juce::Rectangle<int> getInlineLabelBounds();
    [[nodiscard]] juce::Rectangle<int> getLabelBounds();
    [[nodiscard]] juce::Rectangle<int> getValueBackgroundBounds();

    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;
    void textEditorEscapeKeyPressed(juce::TextEditor& editor) override;
    void textEditorFocusLost(juce::TextEditor& editor) override;

    juce::String _label;
    float _minValue;
    float _maxValue;
    float _defaultValue;
    float _currentValue;
    juce::String _valueSuffix;
    int _decimalPlaces;

    juce::RangedAudioParameter* _parameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _attachment;

    juce::TextEditor _textEditor;
    bool _isEditingText = false;

    float _dragStartValue = 0.0f;
    float _dragStartMouseX = 0.0f;
    int _gap = 4;

    std::optional<Theme::FontSize> _fontSizeOverride = std::nullopt;
    std::optional<juce::Colour> _valueBackgroundOverride = std::nullopt;
    std::optional<Theme::ThemeColor> _valueBackgroundThemeColorOverride = std::nullopt;
    std::optional<float> _valueBackgroundWidthOverride = std::nullopt;
    std::optional<float> _valueBackgroundHeightOverride = std::nullopt;
    Alignment _valueBackgroundHorizontalAlignment = START;
    Alignment _valueBackgroundVerticalAlignment = START;
    DisplayMode _displayMode = DisplayMode::Stacked;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueSetter)
};

}
