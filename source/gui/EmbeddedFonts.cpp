#include "../../include/gui/EmbeddedFonts.h"

const juce::Font EmbeddedFonts::getBold()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(ModuleBinaryData::Brandon_bld_otf, ModuleBinaryData::Brandon_bld_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getMedium()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(ModuleBinaryData::Brandon_med_otf, ModuleBinaryData::Brandon_med_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getRegular()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(ModuleBinaryData::Brandon_reg_otf, ModuleBinaryData::Brandon_reg_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getLight()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(ModuleBinaryData::Brandon_light_otf, ModuleBinaryData::Brandon_light_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getThin()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(ModuleBinaryData::Brandon_thin_otf, ModuleBinaryData::Brandon_thin_otfSize);
    return juce::Font(typeface);
}
