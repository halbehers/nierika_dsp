#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Value::Value(const std::string& identifier, const std::string& name, const std::string& unit):
    Component(identifier, name)
{
    addAndMakeVisible(_value);
    _value.setFont(Theme::REGULAR, Theme::SMALL, Theme::FontFamily::MONOSPACED);
    _value.setJustificationType(juce::Justification::centredRight);
    _value.setText("-", false);
    
    _unit.setFont(Theme::THIN, Theme::LABEL);
    refreshUnitColor();
    _unit.setJustificationType(juce::Justification::centredLeft);
    if (unit != "") {
        addAndMakeVisible(_unit);
        _unit.setText(unit, false);
    }
}

Value::~Value()
{
}

void Value::setValue(const std::string& value)
{
    _value.setText(value);
}

std::string Value::getValue() const
{
    return _value.getText();
}

void Value::setUnit(const std::string& unit)
{
    _unit.setText(unit);
}

void Value::paint(juce::Graphics& g)
{
    Component::paint(g);
}

void Value::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    refreshUnitColor();

    Component::changeListenerCallback(source);
}

void Value::refreshUnitColor()
{
    // Text::setColor(juce::Colour) is used here (rather than the ThemeColor overload) for the
    // alpha-dimmed unit text, which clears Text's own ThemeColor-refresh tracking - re-resolve
    // and reapply explicitly on every theme change instead.
    _unit.setColor(Theme::newColor(Theme::TEXT).asJuce().withAlpha(0.7f));
}

void Value::setGap(const float gap)
{
    _gap = gap;
}

void Value::resized()
{
    Component::resized();
    const int x = getLocalBounds().getX();
    const int width = getLocalBounds().getWidth();
    const float realGap = _gap - 4.f;

    _value.setBounds(getLocalBounds().withRight(static_cast<int>(x + (width - width / 2 - realGap / 2))));
    _unit.setBounds(getLocalBounds().withLeft(static_cast<int>(x + (width - width / 2 + realGap / 2))));
}

void Value::setFontSize(Theme::FontSize fontSize)
{
    _value.setFontSize(fontSize);
    _unit.setFontSize(fontSize);
}

void Value::setValueFontSize(Theme::FontSize fontSize)
{
    _value.setFontSize(fontSize);
}

void Value::setUnitFontSize(Theme::FontSize fontSize)
{
    _unit.setFontSize(fontSize);
}

void Value::setFontWeight(Theme::FontWeight weight)
{
    _value.setFontWeight(weight);
    _unit.setFontWeight(weight);
}

void Value::setValueFontWeight(Theme::FontWeight weight)
{
    _value.setFontWeight(weight);
}

void Value::setUnitFontWeight(Theme::FontWeight weight)
{
    _unit.setFontWeight(weight);
}

void Value::setFontFamily(Theme::FontFamily family)
{
    _value.setFontFamily(family);
    _unit.setFontFamily(family);
}

void Value::setValueFontFamily(Theme::FontFamily family)
{
    _value.setFontFamily(family);
}

void Value::setUnitFontFamily(Theme::FontFamily family)
{
    _unit.setFontFamily(family);
}

}
