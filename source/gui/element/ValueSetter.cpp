#include "../../../include/gui/element/ValueSetter.h"

#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

ValueSetter::ValueSetter(const std::string& identifier, const std::string& label, float minValue, float maxValue,
                          float defaultValue, const std::string& valueSuffix, int decimalPlaces):
    Component(identifier),
    _label(label),
    _minValue(minValue),
    _maxValue(maxValue),
    _defaultValue(defaultValue),
    _currentValue(defaultValue),
    _valueSuffix(valueSuffix),
    _decimalPlaces(decimalPlaces)
{
    setup();
}

ValueSetter::ValueSetter(dsp::ParameterManager& parameterManager, const std::string& parameterID,
                          const std::string& valueSuffix, int decimalPlaces):
    Component(parameterID, parameterManager.getParameterDisplayName(parameterID), parameterManager.getParameterTooltip(parameterID)),
    _label(parameterManager.getParameterDisplayName(parameterID)),
    _minValue(parameterManager.getParameterMinValue<float>(parameterID, 0.f)),
    _maxValue(parameterManager.getParameterMaxValue<float>(parameterID, 0.f)),
    _defaultValue(parameterManager.getParameterDefaultValue<float>(parameterID, 0.f)),
    _currentValue(_defaultValue),
    _valueSuffix(valueSuffix),
    _decimalPlaces(decimalPlaces)
{
    setup();

    if (auto* parameter = parameterManager.getState().getParameter(parameterID))
    {
        _parameter = parameter;
        _attachment = std::make_unique<juce::ParameterAttachment>(*parameter,
            [this](float newValue)
            {
                _currentValue = newValue;
                repaint();
            });
        _attachment->sendInitialUpdate();
    }
}

ValueSetter::~ValueSetter()
{
    _textEditor.removeListener(this);
}

void ValueSetter::setup()
{
    setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);

    addChildComponent(_textEditor);
    _textEditor.addListener(this);
    _textEditor.setJustification(juce::Justification::centred);
    _textEditor.setFont(Theme::newFont(Theme::REGULAR, getFontSize()));
    _textEditor.setInputRestrictions(0, "-0123456789.");
    _textEditor.setSelectAllWhenFocused(true);
    _textEditor.setColour(juce::TextEditor::backgroundColourId, Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce());
    _textEditor.setColour(juce::TextEditor::outlineColourId, Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce());
    _textEditor.setColour(juce::TextEditor::focusedOutlineColourId, Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce());
    _textEditor.setColour(juce::TextEditor::textColourId, Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
}

void ValueSetter::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    Component::changeListenerCallback(source);

    // _textEditor's colours/font are cached JUCE ColourIds/Font baked once in setup() - always the
    // same fixed ThemeColors (TRANSPARENT/TEXT), never per-instance customizable, so they can
    // simply be unconditionally re-applied here (getValueBackgroundColour()'s own live-Theme
    // getter and repaint() from above already cover the surrounding pill/label painted in paint()).
    _textEditor.setFont(Theme::newFont(Theme::REGULAR, getFontSize()));
    _textEditor.setColour(juce::TextEditor::backgroundColourId, Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce());
    _textEditor.setColour(juce::TextEditor::outlineColourId, Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce());
    _textEditor.setColour(juce::TextEditor::focusedOutlineColourId, Theme::newColor(Theme::ThemeColor::TRANSPARENT_COLOR).asJuce());
    _textEditor.setColour(juce::TextEditor::textColourId, Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
}

void ValueSetter::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto textColor = Theme::newColor(isEnabled() ? Theme::ThemeColor::TEXT : Theme::ThemeColor::DISABLED).asJuce();

    g.setColour(textColor);
    g.setFont(Theme::newFont(Theme::MEDIUM, getFontSize()));

    const auto labelJustification = _displayMode == DisplayMode::Inline ? juce::Justification::centredRight : juce::Justification::centred;
    g.drawText(_label, getLabelBounds(), labelJustification);

    const auto backgroundBounds = getValueBackgroundBounds();
    g.setColour(getValueBackgroundColour());
    g.fillRoundedRectangle(backgroundBounds.toFloat(), 4.f);

    if (!_isEditingText)
    {
        g.setColour(textColor);
        g.setFont(Theme::newFont(Theme::REGULAR, getFontSize()));
        g.drawText(formatValue(_currentValue), backgroundBounds, juce::Justification::centred);
    }
}

void ValueSetter::resized()
{
    Component::resized();
    _textEditor.setBounds(getValueBackgroundBounds());
}

void ValueSetter::mouseDown(const juce::MouseEvent& event)
{
    // JUCE's disabled state is passive - it doesn't stop mouse events from being delivered to a
    // disabled component or its children, each component has to check isEnabled() itself (this is
    // exactly what juce::Slider/Button/laf::Dial already do). isEnabled() consults the whole
    // ancestor chain, so this correctly blocks interaction when a parent (e.g. OscillatorCurve)
    // gets disabled via Section::setBypass()'s cascade, without ValueSetter needing to know
    // anything about that mechanism itself.
    if (!isEnabled() || _isEditingText)
        return;

    _dragStartValue = _currentValue;
    _dragStartMouseX = event.position.x;

    if (_attachment)
        _attachment->beginGesture();
}

void ValueSetter::mouseDrag(const juce::MouseEvent& event)
{
    if (!isEnabled() || _isEditingText)
        return;

    const auto deltaX = event.position.x - _dragStartMouseX;
    const auto valueDelta = (deltaX / kDragPixelsForFullRange) * (_maxValue - _minValue);
    applyValue(juce::jlimit(_minValue, _maxValue, _dragStartValue + valueDelta), false);
}

void ValueSetter::mouseUp(const juce::MouseEvent&)
{
    if (!isEnabled() || _isEditingText)
        return;

    if (_attachment)
        _attachment->endGesture();
}

void ValueSetter::mouseDoubleClick(const juce::MouseEvent& event)
{
    if (!isEnabled())
        return;

    // Mirrors Dial's double-click-to-reset (juce::Slider::setDoubleClickReturnValue), but scoped
    // to the label rather than the whole component - the value pill's own double-click is already
    // claimed by text-edit entry, so the label is the only free target left for a reset gesture.
    if (getLabelBounds().contains(event.getPosition()))
    {
        applyValue(_defaultValue, true);
        return;
    }

    beginTextEdit();
}

void ValueSetter::setDisplayLabel(const juce::String& label)
{
    _label = label;
    repaint();
}

void ValueSetter::setFontSize(Theme::FontSize size)
{
    _fontSizeOverride = size;
    _textEditor.setFont(Theme::newFont(Theme::REGULAR, getFontSize()));
    repaint();
}

void ValueSetter::resetFontSize()
{
    _fontSizeOverride = std::nullopt;
    _textEditor.setFont(Theme::newFont(Theme::REGULAR, getFontSize()));
    repaint();
}

void ValueSetter::applyValue(float newValue, bool asCompleteGesture)
{
    _currentValue = newValue;

    if (_attachment)
    {
        if (asCompleteGesture)
            _attachment->setValueAsCompleteGesture(newValue);
        else
            _attachment->setValueAsPartOfGesture(newValue);
    }

    repaint();
}

void ValueSetter::beginTextEdit()
{
    _isEditingText = true;
    _textEditor.setText(juce::String(_currentValue, _decimalPlaces), juce::dontSendNotification);
    _textEditor.setVisible(true);
    _textEditor.toFront(true);
    _textEditor.selectAll();
    _textEditor.grabKeyboardFocus();
    repaint();
}

void ValueSetter::commitTextEdit()
{
    const auto parsed = juce::jlimit(_minValue, _maxValue, _textEditor.getText().getFloatValue());
    applyValue(parsed, true);
    _isEditingText = false;
    _textEditor.setVisible(false);
    repaint();
}

void ValueSetter::cancelTextEdit()
{
    _isEditingText = false;
    _textEditor.setVisible(false);
    repaint();
}

juce::String ValueSetter::formatValue(float value) const
{
    return juce::String(value, _decimalPlaces) + _valueSuffix;
}

juce::Rectangle<int> ValueSetter::getPillBounds()
{
    auto bounds = getLocalBounds();

    if (_displayMode == DisplayMode::Inline)
    {
        bounds.removeFromLeft(bounds.getWidth() / 2);
        return bounds.reduced(0, 2).withTrimmedLeft(static_cast<int>(_gap));
    }

    bounds.removeFromTop(static_cast<int>(kLabelHeight) + static_cast<int>(_gap));
    return bounds.reduced(4, 0).withTrimmedBottom(2);
}

juce::Rectangle<int> ValueSetter::getInlineLabelBounds()
{
    auto bounds = getLocalBounds();
    return bounds.removeFromLeft(bounds.getWidth() / 2).withTrimmedRight(static_cast<int>(_gap));
}

juce::Rectangle<int> ValueSetter::getLabelBounds()
{
    if (_displayMode == DisplayMode::Inline)
        return getInlineLabelBounds();

    return getLocalBounds().removeFromTop(static_cast<int>(kLabelHeight));
}

juce::Rectangle<int> ValueSetter::getValueBackgroundBounds()
{
    auto bounds = getPillBounds();

    if (_valueBackgroundWidthOverride.has_value())
    {
        const auto width = juce::jmin(static_cast<int>(*_valueBackgroundWidthOverride), bounds.getWidth());
        const auto extra = bounds.getWidth() - width;
        const auto left = _valueBackgroundHorizontalAlignment == START ? 0
                         : _valueBackgroundHorizontalAlignment == END ? extra
                         : extra / 2;
        bounds = bounds.withX(bounds.getX() + left).withWidth(width);
    }

    if (_valueBackgroundHeightOverride.has_value())
    {
        const auto height = juce::jmin(static_cast<int>(*_valueBackgroundHeightOverride), bounds.getHeight());
        const auto extra = bounds.getHeight() - height;
        const auto top = _valueBackgroundVerticalAlignment == START ? 0
                        : _valueBackgroundVerticalAlignment == END ? extra
                        : extra / 2;
        bounds = bounds.withY(bounds.getY() + top).withHeight(height);
    }

    return bounds;
}

void ValueSetter::textEditorReturnKeyPressed(juce::TextEditor&)
{
    commitTextEdit();
}

void ValueSetter::textEditorEscapeKeyPressed(juce::TextEditor&)
{
    cancelTextEdit();
}

void ValueSetter::textEditorFocusLost(juce::TextEditor&)
{
    commitTextEdit();
}

}
