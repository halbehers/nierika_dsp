#include "../../include/gui/Section.h"

namespace nierika::gui
{

Section::Section(std::string identifier, juce::AudioProcessorValueTreeState& treeState, std::string sectionEnabledParameterID, std::string sectionFXSequencerActivationParameterID):
    _treeState(treeState),
    _identifier(identifier),
    _sectionEnabledParameterID(sectionEnabledParameterID),
    _sectionFXSequencerActivationParameterID(sectionFXSequencerActivationParameterID)
{
    setComponentID(identifier);
    init();
}

Section::~Section()
{
    if (_fxSequencer != nullptr) _fxSequencer->unregisterSection(getID());
}

void Section::setBypassable(bool isBypassable)
{
    _isBypassable = isBypassable;
    _enabledButton.setVisible(_isBypassable);
}

void Section::setBypass(bool isBypassed)
{
    _fxSequencerButton.setEnabled(!isBypassed);
    _nameLabel.setEnabled(!isBypassed);
    bypassComponents(isBypassed);
}

void Section::setFXSequencerActivable(bool isFXSequencerActivable)
{
    _isFXSequencerActivable = isFXSequencerActivable;
    _fxSequencerButton.setVisible(_isFXSequencerActivable);
}

void Section::setSectionName(std::string name)
{
    _nameLabel.setText(Formatter::toUpper(name), juce::NotificationType::dontSendNotification);
    if (name != "")
    {
        addAndMakeVisible(_nameLabel);
        _nameLabel.setFont(EmbeddedFonts::getLight().withHeight(16.0f));
        _nameLabel.setColour(juce::Label::ColourIds::textColourId, Theme::getInstance().getColor(Theme::ThemeColor::TEXT).asJuce());
        _nameLabel.setJustificationType(juce::Justification::centred);
    }
}

void Section::setFXSequencer(dsp::FXSequencer* fxSequencer)
{
    _fxSequencer = fxSequencer;
}

void Section::init()
{
    addChildComponent(_enabledButton, 100);
    _enabledButton.onClick = [this]()
    {
        setBypass(!_enabledButton.getToggleState());
    };
    _enabledButton.setHelpText("Bypass section");
    if (_sectionEnabledParameterID != "")
    {
        _enabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(_treeState, _sectionEnabledParameterID, _enabledButton);
    }

    if (_fxSequencer == nullptr) return;

    addChildComponent(_fxSequencerButton, 100);
    _fxSequencer->registerSection(getID());
    _fxSequencerButton.onClick = [this]()
    {
        _fxSequencer->setSectionActivation(getID(), _fxSequencerButton.getToggleState());
    };
    _fxSequencerButton.setHelpText("Connect to FX Sequencer");
    if (_sectionFXSequencerActivationParameterID != "")
    {
        _fxSequencerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(_treeState, _sectionFXSequencerActivationParameterID, _fxSequencerButton);
    }
}

void Section::withDisplayBorder(bool displayBorder)
{
    _displayBorder = displayBorder;
}

void Section::withDisplayBackground(bool displayBackground)
{
    _displayBackground = displayBackground;
}

void Section::paint (juce::Graphics& g)
{
    juce::Colour whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    if (_displayBackground)
    {
        juce::Colour grayColor = Theme::getInstance().getColor(Theme::ThemeColor::LIGHTER_SHADE).asJuce();
        g.setGradientFill(juce::ColourGradient(whiteColor.withAlpha(0.1f), getWidth() / 2, 0.0, grayColor.withAlpha(0.1f), getWidth() / 2, getHeight(), false));
        g.fillRoundedRectangle(0.0, 0.0, getWidth(), getHeight(), 17.0);
    }
    
    if (_displayBorder)
    {
        g.setColour(whiteColor.withAlpha(0.20f));
        g.drawRoundedRectangle(0.0, 0.0, getWidth(), getHeight(), 16.0, 1.0);
    }
    
    g.setColour(whiteColor);
    g.setFont(juce::FontOptions (14.0f));
}

void Section::resized()
{
    if (_isBypassable)
    {
        int size = 14;
        _enabledButton.setBounds(getWidth() - ((int) 16.f) - size, ((int) 16.f) - size / 2 + 1, size + 1, size);
    }
    if (_isFXSequencerActivable)
    {
        int size = 13;
        _fxSequencerButton.setBounds(getWidth() - ((int) 16.f) - (size * 2 + 4), ((int) 16.f) - size / 2 + 1, size + 1, size);
    }
    if (_nameLabel.getText() != "")
    {
        _nameLabel.setBounds(0, 12, getWidth(), 8);
    }
}

juce::Rectangle<int> Section::getLocalBounds()
{
    if (_nameLabel.getText() != "") {
        return juce::Component::getLocalBounds().withTop(14);
    }
    return juce::Component::getLocalBounds();
}

int Section::getHeaderHeight()
{
    return _nameLabel.getText() != "" ? 10 : 0;
}

std::string Section::getID() const {
    return _identifier;
}

}
