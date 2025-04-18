#pragma once

#include <string>

#include "../Component.h"

namespace nierika::gui::element
{

class Title: public Component
{
public:
    Title(const std::string& title, const bool withGlowEffect = true);
    ~Title() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;

    std::string getTitle() const;
    void setTitle(const std::string& title);

private:
    juce::Label _title;
    juce::GlowEffect _glowEffect;
};

}
