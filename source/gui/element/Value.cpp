#include "../../../include/gui/element/Title.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Value::Value(const std::string& identifier, const std::string& name, const std::string& unit):
    Component(identifier, name)
{
    addAndMakeVisible(_value);
    _value.setFont(Theme::BOLD, Theme::HEADING);
    _value.setJustificationType(juce::Justification::centredRight);
    _value.setText("-", false);
    
    _unit.setFont(Theme::THIN, Theme::HEADING);
    _unit.setColor(Theme::getInstance().getColor(Theme::TEXT).asJuce().withAlpha(0.7f));
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

void Value::setUnit(const std::string& value)
{
    _unit.setText(value);
}

void Value::paint(juce::Graphics& g)
{
    Component::paint(g);
}

void Value::setGap(const float gap)
{
    _gap = gap;
}

void Value::resized()
{
    Component::resized();
    const auto x = getLocalBounds().getX();
    const auto width = getLocalBounds().getWidth();
    _value.setBounds(getLocalBounds().withRight(x + (width - width / 2 - _gap / 2)));
    _unit.setBounds(getLocalBounds().withLeft(x + (width - width / 2 + _gap / 2)));
}

}
