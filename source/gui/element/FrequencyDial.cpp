namespace nierika::gui::element
{
FrequencyDial::FrequencyDial(const std::string& identifier, const std::string& label, float minValue, float maxValue, float defaultValue):
    Dial(identifier, label, minValue, maxValue, defaultValue, "")
{
    FrequencyDial::setup();
}

FrequencyDial::FrequencyDial(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Dial(parameterManager, parameterID, "")
{
    FrequencyDial::setup();
}

FrequencyDial::~FrequencyDial()
{
}

void FrequencyDial::setup()
{
    _slider.textFromValueFunction = [this](double value) { return getTextFromValue(value); };
}

juce::String FrequencyDial::getTextFromValue(double value)
{
    return Formatter::formatFrequency((float) value);
}

}
