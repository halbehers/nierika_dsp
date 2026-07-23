#pragma once

#include "DevicesComboBox.h"
#include "PopupPanel.h"
#include "../Icons.h"

namespace nierika::gui::element
{

class DevicesComboBoxWithConfig: public Component
{
public:

    explicit DevicesComboBoxWithConfig(const std::string& identifier);
    ~DevicesComboBoxWithConfig() override = default;

    void resized() override;

    DevicesComboBox& getComboBox() { return _devicesComboBox; }
    const DevicesComboBox& getComboBox() const { return _devicesComboBox; }

    void setHeightType(Theme::HeightType type);

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
