#include <algorithm>
#include <cmath>

#include "../../include/utils/Logger.h"
#include "../../include/gui/Section.h"

namespace nierika::gui
{

Section::Section(const std::string& identifier, dsp::ParameterManager& parameterManager, const std::string& sectionEnabledParameterID, const std::string& sectionFXSequencerActivationParameterID):
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

    if (!sectionEnabledParameterID.empty()) setBypassable(true);

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

void Section::onToggleValueChanged(const std::string& componentID, bool isOn)
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

    for (auto&[_, components] : _registeredComponentsByPanelID)
        for (auto& component : components)
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
    if (!name.empty())
    {
        addAndMakeVisible(_nameLabel);
        _nameLabel.setFont(Theme::newFont(Theme::MEDIUM, Theme::CAPTION));
        _nameLabel.setColour(juce::Label::ColourIds::textColourId, Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
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
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setGap(gap);
}

void Section::displayBorder()
{
    Component::displayBorder(Theme::ThemeColor::EMPTY_SHADE, 1.f, 17.f, 0.2f);
}

void Section::displayBackground()
{
    const juce::Colour whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    const juce::Colour grayColor = Theme::newColor(Theme::ThemeColor::LIGHTER_SHADE).asJuce();

    Component::displayBackground(juce::ColourGradient(whiteColor.withAlpha(0.1f), getWidth() / 2, 0.0, grayColor.withAlpha(0.1f), getWidth() / 2, getHeight(), false), 17.f);
}

void Section::paint(juce::Graphics& g)
{
    Component::paint(g);
    getActiveLayout().paint(g);
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
        _tabs.setBounds(0, getHeight() - static_cast<int>(FOOTER_HEIGHT), getWidth(), static_cast<int>(FOOTER_HEIGHT));
    }
}

juce::Rectangle<int> Section::getBypassButtonBounds()
{
    constexpr int size = 14;
    return juce::Rectangle<int>(getWidth() - 16 - size, 8, size + 1, size);
}

juce::Rectangle<int> Section::getFXSequencerButtonBounds()
{
    constexpr int size = 13;
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
        utils::AppLogger::warn("Panel ID not found: " + panelID, "Section::removePanel");
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
    if (!_panelLayoutsByID.contains(panelID))
    {
        utils::AppLogger::warn("Panel ID not found: " + panelID, "Section::switchPanel");
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

    if (registeredComponents.empty())
    {
        utils::AppLogger::warn("Call to initLayout() without any registered components.", "Section");
        return 0;
    }

    return std::min(static_cast<int>(registeredComponents.size()), maxNbColumns);
}

int Section::computeNbOfRows(const int maxNbRows, const int nbOfColumns, const std::string& panelID) const
{
    const auto& registeredComponents = getRegisteredComponents(panelID);

    if (registeredComponents.empty())
    {
        utils::AppLogger::warn("Call to initLayout() without any registered components.", "Section");
        return 0;
    }

    return std::min(static_cast<int>(std::ceil(static_cast<float>(registeredComponents.size()) / static_cast<float>(nbOfColumns))), maxNbRows);
}

void Section::initLayout(const int maxNbColumns, const int maxNbRows, const std::string& panelID)
{
    const int nbColumns = computeNbOfColumns(maxNbColumns, panelID);
    std::vector<int> columns;
    columns.reserve(static_cast<std::size_t>(nbColumns));
    for (int i = 0; i < nbColumns; ++i)
        columns.push_back(1);
    const int nbRows = computeNbOfRows(maxNbRows, nbColumns, panelID);
    std::vector<int> rows;
    rows.reserve(static_cast<std::size_t>(nbRows));
    for (int i = 0; i < nbRows; ++i)
        rows.push_back(1);

    auto& layout = getLayout(panelID);
    
    layout.init(rows, columns);

    const auto& registeredComponents = getRegisteredComponents(panelID);
    
    for (std::size_t i = 0; i < registeredComponents.size(); ++i)
    {
        if (static_cast<int>(i) >= nbColumns * nbRows) break;

        const int rowPosition = static_cast<int>(i) / nbColumns;
        const int columnPosition = static_cast<int>(i) % nbColumns;
        layout.addComponent(registeredComponents[i].get().getID(), registeredComponents[i].get(), rowPosition, columnPosition, 1, 1);
    }
}

void Section::initLayout(const std::string& panelID, const int maxNbColumns, const int maxNbRows)
{
    initLayout(maxNbColumns, maxNbRows, panelID);
}

void Section::initLayout(const int maxNbColumns, const int maxNbRows)
{
    for (auto&[id, _] : _panelLayoutsByID)
        initLayout(maxNbColumns, maxNbRows, id);
}

void Section::setLayoutMargin(layout::Spacing<float> margins)
{
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setMargin(margins);
}

void Section::setLayoutMargin(const float marginLeft, const float marginTop, const float marginRight, const float marginBottom)
{
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setMargin(marginLeft, marginTop, marginRight, marginBottom);
}

void Section::setLayoutMargin(const float horizontalMargin, const float verticalMargin)
{
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setMargin(horizontalMargin, verticalMargin);
}

void Section::setLayoutMargin(const float value)
{
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setMargin(value);
}

void Section::setLayoutDisplayGrid(bool displayGrid)
{
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setDisplayGrid(displayGrid);
}

void Section::setLayoutResizableLineConfiguration(layout::GridLayout<Component>::ResizableLineConfiguration configuration)
{
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setResizableLineConfiguration(configuration);
}

void Section::setLayoutMovableConfiguration(layout::GridLayout<Component>::MovableConfiguration configuration)
{
    for (auto&[_, layout] : _panelLayoutsByID)
        layout->setMovableConfiguration(configuration);
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
