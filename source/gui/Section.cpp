#include "../../include/gui/Section.h"

namespace nierika::gui
{

Section::Section(std::string identifier, juce::AudioProcessorValueTreeState& treeState, std::string sectionEnabledParameterID, std::string sectionFXSequencerActivationParameterID):
    Component(identifier),
    _treeState(treeState),
    _layout(*this),
    _sectionEnabledParameterID(sectionEnabledParameterID),
    _sectionFXSequencerActivationParameterID(sectionFXSequencerActivationParameterID)
{
    setPadding(0.f, 16.f, 0.f, 0.f);

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
    _fxSequencer->registerSection(getComponentID().toStdString());
    _fxSequencerButton.onClick = [this]()
    {
        _fxSequencer->setSectionActivation(getComponentID().toStdString(), _fxSequencerButton.getToggleState());
    };
    _fxSequencerButton.setHelpText("Connect to FX Sequencer");
    if (_sectionFXSequencerActivationParameterID != "")
    {
        _fxSequencerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(_treeState, _sectionFXSequencerActivationParameterID, _fxSequencerButton);
    }
}

Section::~Section()
{
    if (_fxSequencer != nullptr) _fxSequencer->unregisterSection(getComponentID().toStdString());
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

void Section::displayBorder()
{
    Component::displayBorder(Theme::ThemeColor::EMPTY_SHADE, 1.f, 17.f, 0.2f);
}

void Section::displayBackground()
{
    juce::Colour whiteColor = Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    juce::Colour grayColor = Theme::getInstance().getColor(Theme::ThemeColor::LIGHTER_SHADE).asJuce();

    Component::displayBackground(juce::ColourGradient(whiteColor.withAlpha(0.1f), getWidth() / 2, 0.0, grayColor.withAlpha(0.1f), getWidth() / 2, getHeight(), false), 17.f);
}

void Section::paint (juce::Graphics& g)
{
    Component::paint(g);
    _layout.paint(g);

    g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());
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

    _layout.resized();
}

int Section::getHeaderHeight()
{
    return _nameLabel.getText() != "" ? 10 : 0;
}

}
