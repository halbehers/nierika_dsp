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

    enum class IconPosition
    {
        LEFT,
        RIGHT
    };

    TextButton(const std::string& identifier, const std::string& text);
    ~TextButton() override = default;

    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void addOnClickListener(OnClickListener* listener);
    void removeListener(OnClickListener* listener);

    void setButtonText(const std::string& text);
    void setHelpText(const std::string& helpText);

    void setIsSelected(const bool isSelected) { _isSelected = isSelected; }
    bool isSelected() const { return _isSelected; }
    
    void setColour(int colourID, juce::Colour newColour);
    void setColour(int colourID, Theme::ThemeColor color);

    void setIcon(const char* svgBinary, IconPosition position = IconPosition::LEFT);
    void resetIcon();
    bool hasIcon() const { return _iconBinary != nullptr; }
    IconPosition getIconPosition() const { return _iconPosition; }
    const char* getIconBinary() const { return _iconBinary; }
    float getIconSize() const { return static_cast<float>(_button.getHeight()) * 0.4f; }
    void setGap(float gap) { _gap = gap; }
    float getGap() const { return _gap; }
    void setInvertedTextColor(bool hasInvertedTextColor) { _hasInvertedTextColor = hasInvertedTextColor; }
    bool hasInvertedTextColor() const { return _hasInvertedTextColor; }

    void setHeightType(Theme::HeightType type) { _heightType = type; resized(); }
    [[nodiscard]] Theme::HeightType getHeightType() const { return _heightType; }

protected:
    juce::TextButton _button {};

private:
    laf::TextButton _lookAndFeel { *this };
    bool _isSelected = true;

    std::vector<OnClickListener*> _listeners;

    const char* _iconBinary = nullptr;
    IconPosition _iconPosition = IconPosition::LEFT;
    float _gap = 8.f;

    bool _hasInvertedTextColor = false;
    Theme::HeightType _heightType = Theme::HeightType::AUTO;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextButton)
};

}
 
