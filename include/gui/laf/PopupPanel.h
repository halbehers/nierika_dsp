#pragma once

namespace nierika::gui::laf
{

// Shared, stateless look-and-feel for element::PopupPanel's CallOutBox - reads only Theme::
// defaults (unlike most LAFs in this module, there's no persistent owning element instance to
// read per-instance overrides from, since a CallOutBox is transient).
class PopupPanel final : public juce::LookAndFeel_V4
{
public:
    PopupPanel() = default;
    ~PopupPanel() override = default;

    void drawCallOutBoxBackground(juce::CallOutBox& box, juce::Graphics& g, const juce::Path& path, juce::Image& cachedImage) override;
    int getCallOutBoxBorderSize(const juce::CallOutBox& box) override;
    float getCallOutBoxCornerSize(const juce::CallOutBox& box) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupPanel)
};

}
