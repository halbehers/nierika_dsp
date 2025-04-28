#pragma once

#include <string>
#include <memory>
#include <vector>

#include "../dsp/sequencer/FXSequencer.h"
#include "./Icons.h"
#include "./element/SVGToggle.h"
#include "./layout/GridLayout.h"
#include "./Component.h"

namespace nierika::gui
{

class Section : public Component, public element::SVGToggle::OnValueChangedListener
{
public:
    Section(std::string identifier, dsp::ParameterManager& parameterManager, std::string sectionEnabledParameterID = "", std::string sectionFXSequencerActivationParameterID = "");
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
    void registerComponent(Component& component);
    void initLayout(const int maxNbColumns = 2, const int maxNbRows = 10);
    
    void setHasHeader(bool hasHeader);
    bool hasHeader() const;

    void onToggleValueChanged(const std::string componentID, bool isOn) override;

protected:
    dsp::FXSequencer* _fxSequencer =  nullptr;
    dsp::ParameterManager& _parameterManager;
    juce::Label _nameLabel;
    
    element::SVGToggle _enabledButton;
    element::SVGToggle _fxSequencerButton;
    bool _hasHeader = false;

    int getHeaderHeight();
    virtual void bypassComponents(bool isBypassed) {};

    layout::GridLayout<Component>& getLayout() { return _layout; }

protected:
    virtual juce::Rectangle<int> getBypassButtonBounds();
    virtual juce::Rectangle<int> getFXSequencerButtonBounds();

private:
    layout::GridLayout<Component> _layout;

    std::string _sectionEnabledParameterID = "";
    std::string _sectionFXSequencerActivationParameterID = "";

    bool _isBypassable = false;
    bool _isFXSequencerActivable = false;
    
    std::vector<std::reference_wrapper<Component>> _registeredComponents;
    
    int computeNbOfColumns(const int maxNbColumns) const;
    int computeNbOfRows(const int maxNbRows, const int nbOfColumns) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Section)
};

}
