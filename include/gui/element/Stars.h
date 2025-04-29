#pragma once

#include "Slider.h"

namespace nierika::gui::element
{

class Stars: public juce::Slider
{
public:
    Stars(int maxWidth, int maxHeight);
    ~Stars() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    int _width;
    int _height;

    std::vector<juce::Point<float>> _smallStarPositions;
    std::vector<juce::Point<float>> _mediumStarPositions;
    std::vector<juce::Point<float>> _largeStarPositions;

    juce::GlowEffect _glowEffect;

    void computeSmallStars();
    void computeMediumStars();
    void computeLargeStars();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Stars)
};

}
