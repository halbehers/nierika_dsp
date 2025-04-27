#pragma once

#include "Slider.h"
#include "../laf/Dial.h"
#include "../Component.h"
#include "../../dsp/ParameterManager.h"
#include "../../dsp/Parameter.h"

namespace nierika::gui::element
{

class Dial: public Component
{
public:
    enum Size
    {
        SMALL,
        MEDIUM,
        LARGE
    };
    
    Dial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue, const std::string& valueSuffix = "", Size size = Size::MEDIUM);
    Dial(const dsp::ParameterManager& parameterManager, const std::string& parameterID, const std::string& valueSuffix = "", Size size = Size::MEDIUM);
    ~Dial() override;

    void resized() override;
    void setSize(Size size);
    void setShortLabel(const juce::String& shortLabel);
    void setEnabled(bool isEnabled);
    
protected:
    float _minValue;
    float _maxValue;
    float _defaultValue;
    std::string _valueSuffix;

    juce::Slider _slider;
    laf::Dial _lookAndFeel;
    Size _size;
    std::unordered_map<Size, float> _sizeToPx {
        {
            { SMALL, 32.0 },
            { MEDIUM, 42.0 },
            { LARGE, 55.0 }
        }
    };

    void setup();
};

}
