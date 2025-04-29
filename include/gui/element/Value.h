#pragma once

#include <string>

#include "../Component.h"
#include "Text.h"

namespace nierika::gui::element
{

class Value: public Component
{
public:
    Value(const std::string& identifier, const std::string& name = "", const std::string& unit = "");
    ~Value() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    std::string getValue() const;
    void setValue(const std::string& value);
    void setUnit(const std::string& unit);
    void setGap(const float gap);

private:
    Text _value = { "value" };
    Text _unit = { "unit" };
    float _gap = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Value)
};

}
