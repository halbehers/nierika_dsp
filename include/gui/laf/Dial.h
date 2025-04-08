#pragma once

namespace nierika::gui::laf
{

class Dial : public juce::LookAndFeel_V4
{
public:
    Dial();
    ~Dial() override;
    
    void drawRotarySlider
    (
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider &slider
    ) override;
            
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
    
    void setShortLabel(const juce::String& shortLabel);
    
private:
    float _sliderWidth;
    bool sliderIsDial = true;
    bool isReversed = false;
    juce::String _sliderName = "";
    juce::String _shortLabel = "";
};

}
