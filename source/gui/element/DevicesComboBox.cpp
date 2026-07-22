#include "../../../include/gui/element/DevicesComboBox.h"

namespace nierika::gui::element
{

namespace
{
    bool matches(const dsp::AudioOutputDeviceInfo& a, const dsp::AudioOutputDeviceInfo& b)
    {
        if (a.kind != b.kind)
            return false;

        return a.kind != dsp::AudioOutputDeviceKind::DEVICE || (a.name == b.name && a.typeName == b.typeName);
    }
}

DevicesComboBox::DevicesComboBox(const std::string& identifier):
    ComboBox(identifier)
{
    refreshDevices(juce::dontSendNotification);
}

void DevicesComboBox::refreshDevices(const juce::NotificationType notification)
{
    const auto previouslySelectedId = getSelectedId();
    const bool hadSelection = previouslySelectedId >= 1 && static_cast<std::size_t>(previouslySelectedId) <= _devices.size();
    const dsp::AudioOutputDeviceInfo previouslySelected = hadSelection ? _devices[static_cast<std::size_t>(previouslySelectedId - 1)] : dsp::AudioOutputDeviceInfo{};

    _devices = dsp::AudioOutputDeviceList::getAllDevices();

    clear(juce::dontSendNotification);
    for (std::size_t i = 0; i < _devices.size(); ++i)
        addItem(dsp::AudioOutputDeviceList::getDisplayName(_devices[i]), static_cast<int>(i) + 1);

    int newSelectedId = 1; // "No Device" is always first
    if (hadSelection)
    {
        for (std::size_t i = 0; i < _devices.size(); ++i)
        {
            if (matches(_devices[i], previouslySelected))
            {
                newSelectedId = static_cast<int>(i) + 1;
                break;
            }
        }
    }

    setSelectedId(newSelectedId, notification);
}

const dsp::AudioOutputDeviceInfo& DevicesComboBox::getSelectedDevice() const
{
    static const dsp::AudioOutputDeviceInfo noDevice{};

    const auto selectedId = getSelectedId();
    if (selectedId < 1 || static_cast<std::size_t>(selectedId) > _devices.size())
        return noDevice;

    return _devices[static_cast<std::size_t>(selectedId - 1)];
}

void DevicesComboBox::setSelectedDevice(const dsp::AudioOutputDeviceKind kind, const juce::String& name, const juce::String& typeName,
                                         const juce::NotificationType notification)
{
    const dsp::AudioOutputDeviceInfo target{ kind, name, typeName, 0, 0, false };

    for (std::size_t i = 0; i < _devices.size(); ++i)
    {
        if (matches(_devices[i], target))
        {
            setSelectedId(static_cast<int>(i) + 1, notification);
            return;
        }
    }

    setSelectedId(1, notification); // fall back to "No Device"
}

}
