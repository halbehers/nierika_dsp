#include <algorithm>
#include <cmath>

#include "../../include/utils/Logger.h"
#include "../../include/gui/Section.h"

namespace nierika::gui
{

Section::Section(std::string identifier, dsp::ParameterManager& parameterManager, std::string sectionEnabledParameterID, std::string sectionFXSequencerActivationParameterID):
    Component(identifier),
    _parameterManager(parameterManager),
    _enabledButton(parameterManager, sectionEnabledParameterID, Icons::getPowerOff()),
    _fxSequencerButton(parameterManager, sectionFXSequencerActivationParameterID, Icons::getBoxes()),
    _sectionEnabledParameterID(sectionEnabledParameterID),
    _sectionFXSequencerActivationParameterID(sectionFXSequencerActivationParameterID)
{
    _panelLayoutsByID.emplace(MAIN_PANEL_ID, std::make_unique<layout::GridLayout<Component>>(*this));

    _tabs.addOnTabChangedListener(this);
    _tabs.addTab(MAIN_PANEL_ID, "Main", "", true);
    
    addChildComponent(_tabs, 100);
    
    setGap(12.f);

    addChildComponent(_enabledButton, 100);
    _enabledButton.addOnValueChangedListener(this);

    if (sectionEnabledParameterID != "") setBypassable(true);

    if (_fxSequencer == nullptr) return;

    addChildComponent(_fxSequencerButton, 100);
    _fxSequencer->registerSection(getComponentID().toStdString());
    _fxSequencerButton.addOnValueChangedListener(this);
}

Section::~Section()
{
    _tabs.removeListener(this);
    _enabledButton.removeListener(this);

    if (_fxSequencer != nullptr) {
        _fxSequencer->unregisterSection(getComponentID().toStdString());
        _fxSequencerButton.removeListener(this);
    }
}

void Section::onToggleValueChanged(const std::string componentID, bool isOn)
{
    if (componentID == _sectionEnabledParameterID)
        setBypass(!isOn);
    else if (componentID == _sectionFXSequencerActivationParameterID)
    {
        _fxSequencer->setSectionActivation(getComponentID().toStdString(), isOn);
    }
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

    for (auto & component : getActiveRegisteredComponents())
        component.get().setEnabled(!isBypassed);

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

void Section::setGap(const float gap)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setGap(gap);
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
    getActiveLayout().paint(g);

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
        _nameLabel.setBounds(0, 10, getWidth(), 8);
    }

    getActiveLayout().resized();

    if (_tabs.count() > 1)
    {
        _tabs.setBounds(0, getY() + getHeight() - static_cast<int>(FOOTER_HEIGHT), getWidth(), static_cast<int>(FOOTER_HEIGHT));
    }
}

juce::Rectangle<int> Section::getBypassButtonBounds()
{
    int size = 14;
    return juce::Rectangle<int>(getWidth() - 16 - size, 8, size + 1, size);
}

juce::Rectangle<int> Section::getFXSequencerButtonBounds()
{
    int size = 13;
    return juce::Rectangle<int>(getWidth() - 16 - (size * 2 + 4), 8, size + 1, size);
}

void Section::addPanel(const std::string& panelID, const std::string &name)
{
    _panelLayoutsByID.emplace(panelID, std::make_unique<layout::GridLayout<Component>>(*this));
    _tabs.addTab(panelID, name);
    
    setGap(12.f);

    const auto showTabs = _tabs.count() > 1;

    setHasFooter(showTabs);
    _tabs.setVisible(showTabs);
}

void Section::removePanel(const std::string& panelID)
{
    auto it = _panelLayoutsByID.find(panelID);
    if (it != _panelLayoutsByID.end())
    {
        _panelLayoutsByID.erase(it);
    }
    else
    {
        utils::AppLogger::get().warn("Panel ID not found: " + panelID, "Section::removePanel");
    }

    _tabs.removeTab(panelID);
    
    const auto showTabs = _tabs.count() > 1;

    setHasFooter(showTabs);
    _tabs.setVisible(showTabs);
}

void Section::setPanelName(const std::string& panelID, const std::string& name)
{
    _tabs.setTabName(panelID, name);
}

std::string Section::getPanelName(const std::string& panelID) const
{
    return _tabs.getTabName(panelID);
}

std::string Section::getActivePanelName() const
{
    return _tabs.getSelectedTabName();
}

void Section::switchPanel(const std::string& panelID)
{
    if (panelID == _selectedPanelID) return;
    if (_panelLayoutsByID.find(panelID) == _panelLayoutsByID.end())
    {
        utils::AppLogger::get().warn("Panel ID not found: " + panelID, "Section::switchPanel");
        return;
    }
    getActiveLayout().setVisible(false);
    _tabs.setSelectedTabID(panelID);
    _selectedPanelID = panelID;
    getActiveLayout().setVisible(true);
}

void Section::onTabChanged(const std::string& newSelectedTabID)
{
    switchPanel(newSelectedTabID);
}

void Section::registerComponent(Component& component, const std::string& panelID)
{
    _registeredComponentsByPanelID[panelID].push_back(component);
}

int Section::computeNbOfColumns(const int maxNbColumns, const std::string& panelID) const
{
    const auto& registeredComponents = getRegisteredComponents(panelID);

    if (registeredComponents.size() == 0)
    {
        utils::AppLogger::get().warn("Call to initLayout() without any registered components.", "Section");
        return 0;
    }

    return std::min(static_cast<int>(registeredComponents.size()), maxNbColumns);
}

int Section::computeNbOfRows(const int maxNbRows, const int nbOfColumns, const std::string& panelID) const
{
    const auto& registeredComponents = getRegisteredComponents(panelID);

    if (registeredComponents.size() == 0)
    {
        utils::AppLogger::get().warn("Call to initLayout() without any registered components.", "Section");
        return 0;
    }

    return std::min(static_cast<int>(std::ceil(registeredComponents.size() / static_cast<float>(nbOfColumns))), maxNbRows);
}

void Section::initLayout(const int maxNbColumns, const int maxNbRows, const std::string& panelID)
{
    const int nbColumns = computeNbOfColumns(maxNbColumns, panelID);
    std::vector<int> columns;
    columns.reserve(nbColumns);
    for (int i = 0; i < nbColumns; ++i)
        columns.push_back(1);
    const int nbRows = computeNbOfRows(maxNbRows, nbColumns, panelID);
    std::vector<int> rows;
    rows.reserve(nbRows);
    for (int i = 0; i < nbRows; ++i)
        rows.push_back(1);

    auto& layout = getLayout(panelID);
    
    layout.init(rows, columns);

    const auto& registeredComponents = getRegisteredComponents(panelID);
    
    for (int i = 0; i < registeredComponents.size(); ++i)
    {
        if (i >= nbColumns * nbRows) break;

        const int rowPosition = i / nbColumns;
        const int columnPosition = i % nbColumns;
        layout.addComponent(registeredComponents[i].get().getID(), registeredComponents[i].get(), rowPosition, columnPosition, 1, 1);
    }
}

void Section::initLayout(const std::string& panelID, const int maxNbColumns, const int maxNbRows)
{
    initLayout(maxNbColumns, maxNbRows, panelID);
}

void Section::initLayout(const int maxNbColumns, const int maxNbRows)
{
    for (auto& layout : _panelLayoutsByID)
        initLayout(maxNbColumns, maxNbRows, layout.first);
}

void Section::setLayoutMargin(layout::Spacing<float> margins)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setMargin(margins);
}

void Section::setLayoutMargin(const float marginLeft, const float marginTop, const float marginRight, const float marginBottom)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setMargin(marginLeft, marginTop, marginRight, marginBottom);
}

void Section::setLayoutMargin(const float horizontalMargin, const float verticalMargin)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setMargin(horizontalMargin, verticalMargin);
}

void Section::setLayoutMargin(const float value)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setMargin(value);
}

void Section::setLayoutDisplayGrid(bool displayGrid)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setDisplayGrid(displayGrid);
}

void Section::setLayoutResizableLineConfiguration(layout::GridLayout<Component>::ResizableLineConfiguration configuration)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setResizableLineConfiguration(configuration);
}

void Section::setLayoutMovableConfiguration(layout::GridLayout<Component>::MovableConfiguration configuration)
{
    for (auto& layout : _panelLayoutsByID)
        layout.second->setMovableConfiguration(configuration);
}

const std::vector<std::reference_wrapper<Component>>& Section::getRegisteredComponents(const std::string& panelID) const
{
        return _registeredComponentsByPanelID.at(panelID);
}

const std::vector<std::reference_wrapper<Component>>& Section::getActiveRegisteredComponents() const
{
    return _registeredComponentsByPanelID.at(_tabs.getSelectedTabID());
}

void Section::setHasHeader(bool hasHeader)
{
    _hasHeader = hasHeader;
    if (_hasHeader) withTopPadding(HEADER_HEIGHT);
    else setPadding(0.f);
}

bool Section::hasHeader() const
{
    return _hasHeader;
}

void Section::setHasFooter(bool hasFooter)
{
    _hasFooter = hasFooter;
    if (_hasFooter) withBottomPadding(FOOTER_HEIGHT);
    else setPadding(0.f);
}

bool Section::hasFooter() const
{
    return _hasFooter;
}

}
