#pragma once

#include <string>

#include "../Component.h"
#include "Text.h"

namespace nierika::gui::element
{

class Value: public Component
{
public:
    explicit Value(const std::string& identifier, const std::string& name = "", const std::string& unit = "");
    ~Value() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    std::string getValue() const;
    void setValue(const std::string& value);
    void setUnit(const std::string& unit);
    void setGap(float gap);
    void setFontSize(Theme::FontSize fontSize);
    void setValueFontSize(Theme::FontSize fontSize);
    void setUnitFontSize(Theme::FontSize fontSize);
    void setFontWeight(Theme::FontWeight weight);
    void setValueFontWeight(Theme::FontWeight weight);
    void setUnitFontWeight(Theme::FontWeight weight);
    void setFontFamily(Theme::FontFamily family);
    void setValueFontFamily(Theme::FontFamily family);
    void setUnitFontFamily(Theme::FontFamily family);

private:
    Text _value = Text("value");
    Text _unit = Text("unit");
    float _gap = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Value)
};

}
