#pragma once

class EmbeddedFonts
{
public:
    static juce::Font getBold(float fontSize = 16.f);
    static juce::Font getMedium(float fontSize = 16.f);
    static juce::Font getRegular(float fontSize = 16.f);
    static juce::Font getLight(float fontSize = 16.f);
    static juce::Font getThin(float fontSize = 16.f);

private:
    static juce::Font buildFont(const void *fontFileData, std::size_t fontFileDataSize, float fontSize = 16.f);
};
