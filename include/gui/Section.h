#pragma once

#include <string>
#include <memory>

#include "../dsp/sequencer/FXSequencer.h"
#include "./Icons.h"
#include "./element/SVGToggle.h"

namespace nierika::gui
{

class Section : public juce::Component
{
public:
    Section(std::string identifier, juce::AudioProcessorValueTreeState& treeState, std::string sectionEnabledParameterID = "", std::string sectionFXSequencerActivationParameterID = "");
    ~Section() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    std::string getID() const;
    
    void withDisplayBorder(bool displayBorder);
    void withDisplayBackground(bool displayBackground);
    void setBypassable(bool isBypassable);
    void setFXSequencerActivable(bool isFXSequencerActivable);
    void setBypass(bool isBypassed);
    juce::Rectangle<int> getLocalBounds();
    
    void setSectionName(std::string name);

    void setFXSequencer(dsp::FXSequencer* fxSequencer);
    
protected:
    dsp::FXSequencer* _fxSequencer =  nullptr;
    juce::AudioProcessorValueTreeState& _treeState;
    nierika::gui::element::SVGToggle _enabledButton { Icons::getInstance().getPowerOff() };
    juce::Label _nameLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _enabledAttachment;
    nierika::gui::element::SVGToggle _fxSequencerButton { Icons::getInstance().getBoxes() };
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _fxSequencerAttachment;
    
    int getHeaderHeight();
    virtual void bypassComponents(bool isBypassed) {};

private:
    const std::string _identifier;
    std::string _sectionEnabledParameterID = "";
    std::string _sectionFXSequencerActivationParameterID = "";
    bool _displayBorder = true;
    bool _displayBackground = true;
    bool _isBypassable = false;
    bool _isFXSequencerActivable = false;
    
    void init();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Section)
};

}
