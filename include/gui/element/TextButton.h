#pragma once

#include "../Component.h"
#include "../laf/TextButton.h"

namespace nierika::gui::element
{

class TextButton: public Component
{
public:    
    struct OnClickListener
    {
        virtual ~OnClickListener() = default;
        virtual void onButtonClick(const std::string& componentID) = 0;
    };

    TextButton(const std::string& identifier, const std::string& text);
    ~TextButton() override = default;

    void resized() override;

    void addOnClickListener(OnClickListener* listener);
    void removeListener(OnClickListener* listener);

    void setButtonText(const std::string& text);
    void setHelpText(const std::string& helpText);

    void setIsSelected(const bool isSelected) { _isSelected = isSelected; }
    bool isSelected() const { return _isSelected; }
    
    void setColour(int colourID, juce::Colour newColour);
    void setColour(int colourID, Theme::ThemeColor color);

private:
    juce::TextButton _button {};
    laf::TextButton _lookAndFeel;
    bool _isSelected = true;

    std::vector<OnClickListener*> _listeners;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextButton)
};

}
 
