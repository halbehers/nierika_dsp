juce::Font EmbeddedFonts::getBold(float fontSize)
{
    return buildFont(NierikaDSPBinaryData::Brandon_bld_otf, NierikaDSPBinaryData::Brandon_bld_otfSize, fontSize);
}

juce::Font EmbeddedFonts::getMedium(float fontSize)
{
    return buildFont(NierikaDSPBinaryData::Brandon_med_otf, NierikaDSPBinaryData::Brandon_med_otfSize, fontSize);
}

juce::Font EmbeddedFonts::getRegular(float fontSize)
{
    return buildFont(NierikaDSPBinaryData::Brandon_reg_otf, NierikaDSPBinaryData::Brandon_reg_otfSize, fontSize);
}

juce::Font EmbeddedFonts::getLight(float fontSize)
{
    return buildFont(NierikaDSPBinaryData::Brandon_light_otf, NierikaDSPBinaryData::Brandon_light_otfSize, fontSize);
}

juce::Font EmbeddedFonts::getThin(float fontSize)
{
    return buildFont(NierikaDSPBinaryData::Brandon_thin_otf, NierikaDSPBinaryData::Brandon_thin_otfSize, fontSize);
}

juce::Font EmbeddedFonts::buildFont(const void *fontFileData, std::size_t fontFileDataSize, float fontSize)
{
    const juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(fontFileData, fontFileDataSize);
    return juce::Font(juce::FontOptions(fontSize).withStyle("").withName("").withTypeface(typeface));
}
