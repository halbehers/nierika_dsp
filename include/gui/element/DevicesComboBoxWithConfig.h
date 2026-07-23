#pragma once

#include "DevicesComboBox.h"
#include "PopupPanel.h"
#include "../Icons.h"

namespace nierika::gui::element
{

// A DevicesComboBox with a gear-icon button next to it that opens a popup (built on PopupPanel)
// for picking which stereo pair of the currently-selected device's output channels the
// AudioDeviceManager should route to. Standalone-only in practice - see setDeviceManager().
class DevicesComboBoxWithConfig: public Component
{
public:

    explicit DevicesComboBoxWithConfig(const std::string& identifier);
    ~DevicesComboBoxWithConfig() override = default;

    void resized() override;

    DevicesComboBox& getComboBox() { return _devicesComboBox; }
    const DevicesComboBox& getComboBox() const { return _devicesComboBox; }

    void setHeightType(Theme::HeightType type);

    // Non-owning. nullptr (non-standalone builds, or before the standalone holder resolves)
    // makes the gear button's click a no-op, as does clicking with no currently-open device.
    void setDeviceManager(juce::AudioDeviceManager* deviceManager) { _deviceManager = deviceManager; }

    void setIconSize(float size) { _settingsButton.setIconSize(size); }
    void setIconBinary(const char* svgBinary) { _settingsButton.setIconBinary(svgBinary); }

    void setIODisplay(dsp::IODisplay ioDisplay) { _devicesComboBox.setIODisplay(ioDisplay); }

private:
    DevicesComboBox _devicesComboBox { "devices-combo-box-with-config-combo-box" };
    PopupPanel _settingsButton { "devices-combo-box-with-config-settings-button", Icons::getGear() };

    juce::AudioDeviceManager* _deviceManager = nullptr;

    static constexpr float SETTINGS_BUTTON_WIDTH = 24.f;
    static constexpr float GAP = 4.f;    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevicesComboBoxWithConfig)
};

}
