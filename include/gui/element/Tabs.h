#pragma once

#include "../Component.h"
#include "../layout/GridLayout.h"
#include "./TextButton.h"

namespace nierika::gui::element
{

class Tabs: public Component, public TextButton::OnClickListener
{
public:
    struct OnTabChangedListener
    {
        virtual ~OnTabChangedListener() = default;
        virtual void onTabChanged(const std::string& newSelectedTabID) = 0;
    };

    Tabs();
    explicit Tabs(const std::string& identifier);
    ~Tabs() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void addOnTabChangedListener(OnTabChangedListener* listener);
    void removeListener(OnTabChangedListener* listener);

    void onButtonClick(const std::string& componentID) override;

    std::string getSelectedTabID() const { return _selectedTabID; }
    void setSelectedTabID(const std::string& id);

    void addTab(const std::string& id, const std::string& name, const std::string& tooltip = "", bool isSelected = false);
    void removeTab(const std::string& id);

    void setTabName(const std::string& id, const std::string& name);
    std::string getTabName(const std::string& id) const;
    std::string getSelectedTabName() const;
    void setTabTooltip(const std::string& id, const std::string& name);
    std::string getTabTooltip(const std::string& id) const;
    std::string getSelectedTabTooltip() const;

    int count() const { return static_cast<int>(_tabs.size()); }

    void reset();

private:
    layout::GridLayout<Component> _layout;
    std::vector<OnTabChangedListener*> _listeners;

    std::vector<std::unique_ptr<TextButton>> _tabs;

    std::string _selectedTabID;

    void setup();

    void initLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tabs)
};

}
 
