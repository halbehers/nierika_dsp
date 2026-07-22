#pragma once

#include "ComboBox.h"

namespace nierika::gui::element
{

class BufferSizeComboBox: public ComboBox
{
public:
    explicit BufferSizeComboBox(const std::string& identifier);
    ~BufferSizeComboBox() override = default;

    void setSelectedBufferSize(int bufferSize, juce::NotificationType notification = juce::sendNotification);
    [[nodiscard]] int getSelectedBufferSize() const;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BufferSizeComboBox)
};

}
