#include "../../../include/gui/element/BufferSizeComboBox.h"

namespace nierika::gui::element
{

namespace
{
    constexpr int BUFFER_SIZES[] = { 32, 64, 128, 256, 512, 1024, 2048 };
}

BufferSizeComboBox::BufferSizeComboBox(const std::string& identifier):
    ComboBox(identifier)
{
    for (const auto bufferSize : BUFFER_SIZES)
        addItem(juce::String(bufferSize) + " Samples", bufferSize);

    setSelectedId(BUFFER_SIZES[3], juce::dontSendNotification); // 256 Samples
}

void BufferSizeComboBox::setSelectedBufferSize(const int bufferSize, const juce::NotificationType notification)
{
    setSelectedId(bufferSize, notification);
}

int BufferSizeComboBox::getSelectedBufferSize() const
{
    return getSelectedId();
}

}
