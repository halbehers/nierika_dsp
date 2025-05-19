#pragma once

#include <string>
#include <memory>
#include <vector>

namespace nierika::gui
{

#define MAIN_PANEL_ID "main"

class Section : public Component, public element::SVGToggle::OnValueChangedListener, public element::Tabs::OnTabChangedListener
{
public:
    Section(const std::string& identifier, dsp::ParameterManager& parameterManager, const std::string& sectionEnabledParameterID = "",  const std::string& sectionFXSequencerActivationParameterID = "");
    ~Section() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void displayBorder();
    void displayBackground();
    void setBypassable(bool isBypassable);
    void setFXSequencerActivable(bool isFXSequencerActivable);
    void setBypass(bool isBypassed);

    void setSectionName(const std::string& name);

    void setFXSequencer(dsp::FXSequencer* fxSequencer);
    void registerComponent(Component& component, const std::string& panelID = MAIN_PANEL_ID);
    void initLayout(int maxNbColumns, int maxNbRows, const std::string& panelID = MAIN_PANEL_ID);
    void initLayout(const std::string& panelID, int maxNbColumns = 2, int maxNbRows = 10);
    void initLayout(int maxNbColumns = 2, int maxNbRows = 10);
    layout::GridLayout<Component>& getLayout(const std::string& panelID = MAIN_PANEL_ID) { return *_panelLayoutsByID[panelID]; }
    layout::GridLayout<Component>& getActiveLayout() { return *_panelLayoutsByID[_selectedPanelID]; }

    void setLayoutMargin(layout::Spacing<float> margins);
    void setLayoutMargin(float marginLeft, float marginTop, float marginRight, float marginBottom);
    void setLayoutMargin(float horizontalMargin, float verticalMargin);
    void setLayoutMargin(float value);

    void setLayoutDisplayGrid(bool displayGrid);
    void setLayoutResizableLineConfiguration(layout::GridLayout<Component>::ResizableLineConfiguration configuration);
    void setLayoutMovableConfiguration(layout::GridLayout<Component>::MovableConfiguration configuration);

    void setHasHeader(bool hasHeader);
    bool hasHeader() const;

    void setHasFooter(bool hasFooter);
    bool hasFooter() const;

    void onToggleValueChanged(const std::string& componentID, bool isOn) override;
    
    void addPanel(const std::string& panelID, const std::string &name);
    void setPanelName(const std::string& panelID, const std::string& name);
    void removePanel(const std::string& panelID);
    std::string getPanelName(const std::string& panelID) const;
    std::string getActivePanelName() const;
    
    void switchPanel(const std::string& panelID);

    void onTabChanged(const std::string& newSelectedTabID) override;

protected:
    dsp::FXSequencer* _fxSequencer =  nullptr;
    dsp::ParameterManager& _parameterManager;
    juce::Label _nameLabel {};
    
    element::SVGToggle _enabledButton;
    element::SVGToggle _fxSequencerButton;

    static constexpr float HEADER_HEIGHT = 30.f;
    static constexpr float FOOTER_HEIGHT = 32.f;

    bool _hasHeader = false;
    bool _hasFooter = false;

    virtual void bypassComponents(bool isBypassed) { (void) isBypassed; }

    void setGap(float gap);
    const std::vector<std::reference_wrapper<Component>>& getRegisteredComponents(const std::string& panelID = MAIN_PANEL_ID) const;
    const std::vector<std::reference_wrapper<Component>>& getActiveRegisteredComponents() const;

    virtual juce::Rectangle<int> getBypassButtonBounds();
    virtual juce::Rectangle<int> getFXSequencerButtonBounds();

private:
    std::unordered_map<std::string, std::unique_ptr<layout::GridLayout<Component>>> _panelLayoutsByID;
    element::Tabs _tabs;
    std::string _selectedPanelID = MAIN_PANEL_ID;

    std::string _sectionEnabledParameterID;
    std::string _sectionFXSequencerActivationParameterID;

    bool _isBypassable = false;
    bool _isFXSequencerActivable = false;

    std::unordered_map<std::string, std::vector<std::reference_wrapper<Component>>> _registeredComponentsByPanelID;

    int computeNbOfColumns(int maxNbColumns, const std::string& panelID = MAIN_PANEL_ID) const;
    int computeNbOfRows(int maxNbRows, int nbOfColumns, const std::string& panelID = MAIN_PANEL_ID) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Section)
};

}
