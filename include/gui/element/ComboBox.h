#pragma once

#include "../Component.h"
#include "../laf/ComboBox.h"

namespace nierika::gui::element
{

class ComboBox: public Component
{
public:
    struct OnValueChangedListener
    {
        virtual ~OnValueChangedListener() = default;
        virtual void onSelectionChanged(const std::string& componentID, int selectedId) = 0;
    };

    explicit ComboBox(const std::string& identifier);
    ~ComboBox() override = default;

    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void addItem(const juce::String& newItemText, int newItemId);
    void setSelectedId(int newItemId, juce::NotificationType notification = juce::sendNotification);
    [[nodiscard]] int getSelectedId() const;

    void addOnValueChangedListener(OnValueChangedListener* listener);
    void removeListener(OnValueChangedListener* listener);

    void setSelectedInvertedTextColor(bool hasInvertedTextColorOnSelected) { _hasInvertedTextColorOnSelected = hasInvertedTextColorOnSelected; }
    bool hasInvertedTextColorOnSelected() const { return _hasInvertedTextColorOnSelected; }

    void setHeightType(Theme::HeightType type) { _heightType = type; resized(); }
    [[nodiscard]] Theme::HeightType getHeightType() const { return _heightType; }

private:
    laf::ComboBox _lookAndFeel { *this };
    juce::ComboBox _comboBox {};
    bool _hasInvertedTextColorOnSelected = false;
    Theme::HeightType _heightType = Theme::HeightType::AUTO;

    std::vector<OnValueChangedListener*> _listeners;

    void setup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBox)
};

}
