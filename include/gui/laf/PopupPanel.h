#pragma once

namespace nierika::gui::laf
{

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
