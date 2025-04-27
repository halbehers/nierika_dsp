#include <algorithm>
#include <cmath>

#include "../../include/utils/Logger.h"
#include "../../include/gui/Section.h"

namespace nierika::gui
{

Section::Section(std::string identifier, dsp::ParameterManager& parameterManager, std::string sectionEnabledParameterID, std::string sectionFXSequencerActivationParameterID):
    Component(identifier),
    _parameterManager(parameterManager),
    _layout(*this),
    _sectionEnabledParameterID(sectionEnabledParameterID),
    _sectionFXSequencerActivationParameterID(sectionFXSequencerActivationParameterID)
{
    getLayout().setGap(16.f);

    addChildComponent(_enabledButton, 100);
    _enabledButton.onClick = [this]()
    {
        setBypass(!_enabledButton.getToggleState());
    };
    _enabledButton.setHelpText("Bypass section");
    if (_sectionEnabledParameterID != "")
    {
        _enabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(_parameterManager.getState(), _sectionEnabledParameterID, _enabledButton);
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
        _fxSequencerAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(_parameterManager.getState(), _sectionFXSequencerActivationParameterID, _fxSequencerButton);
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
    setHasHeader(true);
}

void Section::setBypass(bool isBypassed)
{
    _fxSequencerButton.setEnabled(!isBypassed);
    _nameLabel.setEnabled(!isBypassed);

    for (auto & component : _registeredComponents)
    {
        component.get().setEnabled(!isBypassed);
    }
    bypassComponents(isBypassed);
}

void Section::setFXSequencerActivable(bool isFXSequencerActivable)
{
    _isFXSequencerActivable = isFXSequencerActivable;
    _fxSequencerButton.setVisible(_isFXSequencerActivable);
    setHasHeader(true);
}

void Section::setSectionName(const std::string& name)
{
    _nameLabel.setText(Formatter::toUpper(name), juce::NotificationType::dontSendNotification);
    if (name != "")
    {
        addAndMakeVisible(_nameLabel);
        _nameLabel.setFont(EmbeddedFonts::getLight().withHeight(16.0f));
        _nameLabel.setColour(juce::Label::ColourIds::textColourId, Theme::getInstance().getColor(Theme::ThemeColor::TEXT).asJuce());
        _nameLabel.setJustificationType(juce::Justification::centred);
        setHasHeader(true);
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

void Section::paint(juce::Graphics& g)
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
        _enabledButton.setBounds(getBypassButtonBounds());
    }
    if (_isFXSequencerActivable)
    {
        _fxSequencerButton.setBounds(getFXSequencerButtonBounds());
    }
    if (_nameLabel.getText() != "")
    {
        _nameLabel.setBounds(0, 6, getWidth(), 8);
    }

    _layout.resized();
}

juce::Rectangle<int> Section::getBypassButtonBounds()
{
    int size = 14;
    return juce::Rectangle<int>(getWidth() - 16 - size, 4, size + 1, size);
}

juce::Rectangle<int> Section::getFXSequencerButtonBounds()
{
    int size = 13;
    return juce::Rectangle<int>(getWidth() - 16 - (size * 2 + 4), 4, size + 1, size);
}

int Section::getHeaderHeight()
{
    return _nameLabel.getText() != "" ? 10 : 0;
}

void Section::registerComponent(Component& component)
{
    _registeredComponents.push_back(component);
}

int Section::computeNbOfColumns(const int maxNbColumns) const
{
    if (_registeredComponents.size() == 0)
    {
        utils::AppLogger::get().warn("Call to initLayout() without any registered components.", "Section");
        return 0;
    }

    return std::min(static_cast<int>(_registeredComponents.size()), maxNbColumns);
}

int Section::computeNbOfRows(const int maxNbRows, const int nbOfColumns) const
{
    if (_registeredComponents.size() == 0)
    {
        utils::AppLogger::get().warn("Call to initLayout() without any registered components.", "Section");
        return 0;
    }

    return std::min(static_cast<int>(std::ceil(_registeredComponents.size() / static_cast<float>(nbOfColumns))), maxNbRows);
}

void Section::initLayout(const int maxNbColumns, const int maxNbRows)
{
    const int nbColumns = computeNbOfColumns(maxNbColumns);
    std::vector<int> columns;
    columns.reserve(nbColumns);
    for (int i = 0; i < nbColumns; ++i)
        columns.push_back(1);
    const int nbRows = computeNbOfRows(maxNbRows, nbColumns);
    std::vector<int> rows;
    rows.reserve(nbRows);
    for (int i = 0; i < nbRows; ++i)
        rows.push_back(1);
    
    getLayout().init(rows, columns);
    
    for (int i = 0; i < _registeredComponents.size(); ++i)
    {
        if (i >= nbColumns * nbRows) break;

        const int rowPosition = i / nbColumns;
        const int columnPosition = i % nbColumns;
        getLayout().addComponent(_registeredComponents[i].get().getID(), _registeredComponents[i].get(), rowPosition, columnPosition, 1, 1);
    }
}

void Section::setHasHeader(bool hasHeader)
{
    _hasHeader = hasHeader;
    if (_hasHeader) setPadding(0.f, 16.f, 0.f, 0.f);
    else setPadding(0.f);
}

bool Section::hasHeader() const
{
    return _hasHeader;
}

}
