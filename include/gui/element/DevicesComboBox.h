#pragma once

#include <vector>

#include "ComboBox.h"
#include "../../dsp/AudioOutputDeviceList.h"

namespace nierika::gui::element
{

class DevicesComboBox: public ComboBox
{
public:
    explicit DevicesComboBox(const std::string& identifier);
    ~DevicesComboBox() override = default;

    // Re-scans available output devices (dsp::AudioOutputDeviceList::getAllDevices()) and rebuilds
    // the item list. Tries to preserve the current selection by matching its (kind, name, typeName)
    // against the refreshed list; falls back to the "No Device" sentinel (always id 1) if the
    // previously-selected device is no longer present.
    void refreshDevices(juce::NotificationType notification = juce::dontSendNotification);

    [[nodiscard]] const dsp::AudioOutputDeviceInfo& getSelectedDevice() const;

    void setSelectedDevice(dsp::AudioOutputDeviceKind kind, const juce::String& name, const juce::String& typeName,
                            juce::NotificationType notification = juce::sendNotification);

private:
    std::vector<dsp::AudioOutputDeviceInfo> _devices;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevicesComboBox)
};

}
