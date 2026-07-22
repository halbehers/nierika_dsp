#pragma once

#include "ComboBox.h"

namespace nierika::gui::element
{

class SampleRateComboBox: public ComboBox
{
public:
    explicit SampleRateComboBox(const std::string& identifier);
    ~SampleRateComboBox() override = default;

    void setSelectedSampleRate(double sampleRate, juce::NotificationType notification = juce::sendNotification);
    [[nodiscard]] double getSelectedSampleRate() const;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleRateComboBox)
};

}
