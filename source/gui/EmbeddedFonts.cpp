#include "../../includes/gui/EmbeddedFonts.h"

const juce::Font EmbeddedFonts::getBrandonBold()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Brandon_bld_otf, BinaryData::Brandon_bld_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getBrandonMedium()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Brandon_med_otf, BinaryData::Brandon_med_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getBrandonRegular()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Brandon_reg_otf, BinaryData::Brandon_reg_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getBrandonLight()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Brandon_light_otf, BinaryData::Brandon_light_otfSize);
    return juce::Font(typeface);
}

const juce::Font EmbeddedFonts::getBrandonThin()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::Brandon_thin_otf, BinaryData::Brandon_thin_otfSize);
    return juce::Font(typeface);
}
