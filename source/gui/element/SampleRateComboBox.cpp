#include "../../../include/gui/element/SampleRateComboBox.h"

namespace nierika::gui::element
{

namespace
{
    constexpr int SAMPLE_RATES[] = { 44100, 48000, 88200, 96000 };
}

SampleRateComboBox::SampleRateComboBox(const std::string& identifier):
    ComboBox(identifier)
{
    for (const auto rate : SAMPLE_RATES)
        addItem(juce::String(rate) + " Hz", rate);

    setSelectedId(SAMPLE_RATES[1], juce::dontSendNotification); // 48000 Hz
}

void SampleRateComboBox::setSelectedSampleRate(const double sampleRate, const juce::NotificationType notification)
{
    setSelectedId(static_cast<int>(sampleRate), notification);
}

double SampleRateComboBox::getSelectedSampleRate() const
{
    return static_cast<double>(getSelectedId());
}

}
