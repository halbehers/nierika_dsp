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

    setSectionNameFont(_sectionNameFontWeight, _sectionNameFontSize);
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
        _nameLabel.setFont(Theme::newFont(_sectionNameFontWeight, _sectionNameFontSize));
        _nameLabel.setColour(juce::Label::ColourIds::textColourId, Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
        _nameLabel.setJustificationType(juce::Justification::centred);
        setHasHeader(true);
    }
}

void Section::setSectionNameFontSize(Theme::FontSize fontSize)
{
    _sectionNameFontSize = fontSize;
    setSectionNameFont(_sectionNameFontWeight, _sectionNameFontSize);
}

void Section::setSectionNameFontWeight(Theme::FontWeight fontWeight)
{
    _sectionNameFontWeight = fontWeight;
    setSectionNameFont(_sectionNameFontWeight, _sectionNameFontSize);
}

void Section::setSectionNameFont(Theme::FontWeight fontWeight, Theme::FontSize fontSize)
{
    _nameLabel.setFont(Theme::newFont(fontWeight, fontSize));
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
    Component::displayBorder(Theme::ThemeColor::BORDER, 1.f, Theme::getBorderRadius(), 1.f);
}

void Section::displayBackground()
{
    const juce::Colour whiteColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    const juce::Colour grayColor = Theme::newColor(Theme::ThemeColor::LIGHTER_SHADE).asJuce();

    Component::displayBackground(juce::ColourGradient(whiteColor.withAlpha(0.1f), static_cast<float>(getWidth() / 2), 0.0f, grayColor.withAlpha(0.1f), static_cast<float>(getWidth() / 2), static_cast<float>(getHeight()), false), Theme::getBorderRadius());
}

void Section::paint(juce::Graphics& g)
{
    Component::paint(g);
    getActiveLayout().paint(g);
}

void Section::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    Component::changeListenerCallback(source);

    // _nameLabel is a raw juce::Label (not a nierika::gui::Component), so its colour/font are
    // one-shot juce::Colour/juce::Font values baked in by setSectionName()/setSectionNameFont() -
    // they never track the theme on their own and need re-deriving here on every notification,
    // same as Component's own _border/_background handling above.
    if (_nameLabel.getText() != "")
    {
        _nameLabel.setColour(juce::Label::ColourIds::textColourId, Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
        setSectionNameFont(_sectionNameFontWeight, _sectionNameFontSize);
    }
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
        // Full header height (not a tiny fixed 8px sliver) so the label's own centred
        // justification actually has room to render without clipping top/bottom.
        _nameLabel.setBounds(0, 0, getWidth(), static_cast<int>(HEADER_HEIGHT));
    }

    getActiveLayout().resized();

    if (_tabs.count() > 1)
    {
        const auto footerHeight = getFooterHeight();
        _tabs.setBounds(0, getHeight() - static_cast<int>(footerHeight), getWidth(), static_cast<int>(footerHeight));
    }

    // These four are all added by this base class's own constructor, which - being a base class
    // constructor - always runs before any derived class's own constructor body gets a chance to
    // addAndMakeVisible() its real content. addAndMakeVisible()'s default zOrder (-1) inserts new
    // children at the *front* of the z-order (confirmed against JUCE's own Component::
    // addChildComponent doc and its getComponentAt hit-test traversal, which checks front-to-back
    // - i.e. last-added first), so every derived section's own content silently ends up in front
    // of these header/footer controls, intercepting clicks in any overlapping region even without
    // any visible corruption (the controls still paint fine since paint order matches hit-test
    // order - only interaction breaks). Reasserting front position here, on every resize, is the
    // standard, idiomatic JUCE fix for "these overlay controls must always stay on top."
    //
    // Order matters: toFront() puts each call's target at the very front, so whichever of these
    // runs LAST ends up frontmost. _nameLabel spans the full header width (including the area
    // under _enabledButton/_fxSequencerButton), so it must be sent to front FIRST - otherwise it
    // silently sits on top of the header buttons and swallows their clicks (a disabled _nameLabel
    // still intercepts the hit-test even though it visibly does nothing, since JUCE hit-testing
    // only checks visibility, not isEnabled()).
    if (_nameLabel.getText() != "") _nameLabel.toFront(false);
    if (_tabs.count() > 1) _tabs.toFront(false);
    if (_isBypassable) _enabledButton.toFront(false);
    if (_isFXSequencerActivable) _fxSequencerButton.toFront(false);
}

float Section::getFooterHeight() const
{
    // Theme::resolveHeight only resolves the tab buttons' own row height - it knows nothing
    // about Tabs' internal top/bottom margin, which still has to fit inside whatever we reserve
    // here or the buttons overflow the strip we hand back to them.
    const auto tabsMargin = _tabs.getMargin();
    return Theme::resolveHeight(_tabs.getHeightType(), FOOTER_HEIGHT) + tabsMargin.top + tabsMargin.bottom;
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

    // _selectedPanelID must be updated *before* _tabs.setSelectedTabID(), not after: that call
    // synchronously fires Tabs::OnTabChangedListener, which Section itself implements as
    // onTabChanged() -> switchPanel(panelID) - a reentrant call. If _selectedPanelID were still the
    // old value at that point, the reentrancy guard above wouldn't catch it, and the whole body
    // (including the listener notification below) would run twice per external switchPanel() call.
    _selectedPanelID = panelID;
    _tabs.setSelectedTabID(panelID);
    getActiveLayout().setVisible(true);

    // A newly-activated panel's grid has never had resized() called on it while it was the
    // active one (Section::resized() only ever calls getActiveLayout().resized(), so an inactive
    // panel's items sit at their default (0,0,0,0) bounds until now) - without this, switching to
    // a panel whose content was registered before the Section itself was ever resized leaves
    // every item in that panel invisible-by-zero-size until the next unrelated window resize.
    getActiveLayout().resized();

    for (auto* listener : _panelChangedListeners)
        listener->onPanelChanged(panelID);
}

void Section::addOnPanelChangedListener(OnPanelChangedListener* listener)
{
    _panelChangedListeners.push_back(listener);
}

void Section::removeListener(OnPanelChangedListener* listener)
{
    _panelChangedListeners.erase(std::remove(_panelChangedListeners.begin(), _panelChangedListeners.end(), listener), _panelChangedListeners.end());
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

void Section::setTabsBackgroundColour(juce::Colour colour)
{
    _tabs.setBackgroundColour(colour);
}

void Section::setTabsBackgroundColour(Theme::ThemeColor colorId)
{
    _tabs.setBackgroundColour(colorId);
}

void Section::resetTabsBackgroundColour()
{
    _tabs.resetBackgroundColour();
}

juce::Colour Section::getTabsBackgroundColour() const
{
    return _tabs.getBackgroundColour();
}

void Section::setTabsSelectedBackgroundColour(juce::Colour colour)
{
    _tabs.setSelectedBackgroundColour(colour);
}

void Section::setTabsSelectedBackgroundColour(Theme::ThemeColor colorId)
{
    _tabs.setSelectedBackgroundColour(colorId);
}

void Section::resetTabsSelectedBackgroundColour()
{
    _tabs.resetSelectedBackgroundColour();
}

juce::Colour Section::getTabsSelectedBackgroundColour() const
{
    return _tabs.getSelectedBackgroundColour();
}

void Section::setTabsBorderColour(juce::Colour colour)
{
    _tabs.setBorderColour(colour);
}

void Section::resetTabsBorderColour()
{
    _tabs.resetBorderColour();
}

juce::Colour Section::getTabsBorderColour() const
{
    return _tabs.getBorderColour();
}

void Section::setTabsSelectedBorderColour(juce::Colour colour)
{
    _tabs.setSelectedBorderColour(colour);
}

void Section::resetTabsSelectedBorderColour()
{
    _tabs.resetSelectedBorderColour();
}

juce::Colour Section::getTabsSelectedBorderColour() const
{
    return _tabs.getSelectedBorderColour();
}

void Section::setTabsBorderRadius(float radius)
{
    _tabs.setBorderRadius(radius);
}

void Section::resetTabsBorderRadius()
{
    _tabs.resetBorderRadius();
}

float Section::getTabsBorderRadius() const
{
    return _tabs.getBorderRadius();
}

void Section::setTabsFontSize(Theme::FontSize size)
{
    _tabs.setFontSize(size);
}

void Section::resetTabsFontSize()
{
    _tabs.resetFontSize();
}

Theme::FontSize Section::getTabsFontSize() const
{
    return _tabs.getFontSize();
}

void Section::setTabsFontWeight(Theme::FontWeight weight)
{
    _tabs.setFontWeight(weight);
}

void Section::resetTabsFontWeight()
{
    _tabs.resetFontWeight();
}

Theme::FontWeight Section::getTabsFontWeight() const
{
    return _tabs.getFontWeight();
}

void Section::setTabsFont(Theme::FontWeight weight, Theme::FontSize size)
{
    _tabs.setFont(weight, size);
}

void Section::resetTabsFont()
{
    _tabs.resetFont();
}

void Section::setTabsHeightType(Theme::HeightType type)
{
    _tabs.setHeightType(type);

    // The footer's own padding was sized off the previous height, so it needs recomputing
    // against the new one - otherwise the tab bar and the padding reserved for it drift apart.
    if (_hasFooter) setHasFooter(true);
    resized();
}

Theme::HeightType Section::getTabsHeightType() const
{
    return _tabs.getHeightType();
}

void Section::setTabsDesign(Theme::TabDesign design)
{
    _tabs.setTabDesign(design);

    // Margin (part of Tabs' own outer bounds) changes with design, and getFooterHeight() reads
    // it - recompute the reserved footer padding exactly like setTabsHeightType does.
    if (_hasFooter) setHasFooter(true);
    resized();
}

Theme::TabDesign Section::getTabsDesign() const
{
    return _tabs.getTabDesign();
}

void Section::setTabsTextColour(juce::Colour colour)
{
    _tabs.setTextColour(colour);
}

void Section::resetTabsTextColour()
{
    _tabs.resetTextColour();
}

juce::Colour Section::getTabsTextColour() const
{
    return _tabs.getTextColour();
}

void Section::setTabsSelectedTextColour(juce::Colour colour)
{
    _tabs.setSelectedTextColour(colour);
}

void Section::resetTabsSelectedTextColour()
{
    _tabs.resetSelectedTextColour();
}

juce::Colour Section::getTabsSelectedTextColour() const
{
    return _tabs.getSelectedTextColour();
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
    if (_hasFooter) withBottomPadding(getFooterHeight());
    else setPadding(0.f);
}

bool Section::hasFooter() const
{
    return _hasFooter;
}

}
