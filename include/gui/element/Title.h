#pragma once

#include <string>

namespace nierika::gui::element
{

class Title: public Component
{
public:
    explicit Title(const std::string& title, bool withGlowEffect = true);
    ~Title() override = default;
    
    void paint(juce::Graphics& g) override;
    void resized() override;

    std::string getTitle() const;
    void setTitle(const std::string& title, bool triggerRepaint = true);

private:
    juce::Label _title {};
    juce::GlowEffect _glowEffect {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Title)
};

}
