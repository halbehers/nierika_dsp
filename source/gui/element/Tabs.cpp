#include "../../../include/gui/element/Tabs.h"

namespace nierika::gui::element
{

Tabs::Tabs(const std::string& identifier):
    Component(identifier),
    _layout(*this)
{
    setup();
}

Tabs::Tabs():
    Component(),
    _layout(*this)
{
    setup();
}

void Tabs::setup()
{
    applyDesignLayout();
}

void Tabs::applyDesignLayout()
{
    if (_design == Theme::TabDesign::TAB)
    {
        setMargin(0.f);
        _layout.setGap(0.f);
    }
    else
    {
        setMargin(12.f, 4.f, 12.f, 8.f);
        _layout.setGap(8.f);
    }
}

void Tabs::paint(juce::Graphics& g)
{
    Component::paint(g);
    _layout.paint(g);
}

void Tabs::resized()
{
    Component::resized();
    _layout.resized();
}

void Tabs::initLayout()
{
    std::vector<int> columns;

    const std::size_t nbOfColumns = std::max(_tabs.size(), static_cast<std::size_t>(1));

    columns.reserve(nbOfColumns);
    for (std::size_t i = 0; i < nbOfColumns; ++i)
        columns.push_back(1);

    _layout.init({ 1 }, columns);

    for (std::size_t i = 0; i < _tabs.size(); ++i)
        _layout.addComponent(_tabs[i]->getName().toStdString(), *_tabs[i], 0, static_cast<int>(i), 1, 1);
}

void Tabs::setSelectedTabID(const std::string& id)
{
    if (_selectedTabID == id) return;

    _selectedTabID = id;

    reset();

    for (const auto listener : _listeners)
        listener->onTabChanged(id);
}

void Tabs::reset()
{
    for (const auto& tab : _tabs)
    {
        tab->setIsSelected(tab->getID() == _selectedTabID);
        tab->setEnabled(tab->getID() != _selectedTabID);
    }

    // Which tab sits next to the selected one just changed - applyStyle() recomputes each tab's
    // notch (see there), so re-run it now rather than waiting for an unrelated style setter.
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::onButtonClick(const std::string& componentID)
{
    setSelectedTabID(componentID);
}

void Tabs::addTab(const std::string& id, const std::string& name, const std::string& tooltip, bool isSelected)
{
    auto button = std::make_unique<TextButton>(id, name);
    button->setTooltip(tooltip);
    button->setIsSelected(isSelected);
    if (isSelected) setSelectedTabID(id);

    button->addOnClickListener(this);

    applyStyle(*button);

    _tabs.push_back(std::move(button));

    reset();
    initLayout();
    resized();
    repaint();
}

void Tabs::removeTab(const std::string& id)
{
    if (const auto it = std::remove_if(_tabs.begin(), _tabs.end(), [&id](const auto& tab) { return tab->getID() == id; }); it != _tabs.end())
    {
        _tabs.erase(it, _tabs.end());
        if (_selectedTabID == id) setSelectedTabID(_tabs.empty() ? "" : _tabs[0]->getID());
    }
}

void Tabs::setTabName(const std::string& id, const std::string& name)
{
    for (const auto& tab : _tabs)
    {
        if (tab->getID() == id)
        {
            tab->setName(name);
            tab->setButtonText(name);
            break;
        }
    }
}

std::string Tabs::getTabName(const std::string& id) const
{
    for (const auto& tab : _tabs)
    {
        if (tab->getID() == id)
            return tab->getName().toStdString();
    }
    return "";
}

std::string Tabs::getSelectedTabName() const
{
    return getTabName(_selectedTabID);
}

void Tabs::setTabTooltip(const std::string& id, const std::string& tooltip)
{
    for (const auto& tab : _tabs)
    {
        if (tab->getID() == id)
        {
            tab->setTooltip(tooltip);
            break;
        }
    }
}

std::string Tabs::getTabTooltip(const std::string& id) const
{
    for (const auto& tab : _tabs)
    {
        if (tab->getID() == id)
            return tab->getTooltip();
    }
    return "";
}

std::string Tabs::getSelectedTabTooltip() const
{
    return getTabTooltip(_selectedTabID);
}

void Tabs::setBackgroundColour(juce::Colour colour)
{
    _backgroundOverride = colour;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetBackgroundColour()
{
    _backgroundOverride = juce::Colour();
    for (const auto& tab : _tabs) applyStyle(*tab);
}

juce::Colour Tabs::getBackgroundColour() const
{
    return _backgroundOverride.value_or(Theme::newColor(Theme::ThemeColor::LIGHT_SHADE).asJuce());
}

void Tabs::setSelectedBackgroundColour(juce::Colour colour)
{
    _selectedBackgroundOverride = colour;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetSelectedBackgroundColour()
{
    _selectedBackgroundOverride = juce::Colour();
    for (const auto& tab : _tabs) applyStyle(*tab);
}

juce::Colour Tabs::getSelectedBackgroundColour() const
{
    return _selectedBackgroundOverride.value_or(getBackgroundColour());
}

void Tabs::setBorderColour(juce::Colour colour)
{
    _borderOverride = colour;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetBorderColour()
{
    _borderOverride = juce::Colour();
    for (const auto& tab : _tabs) applyStyle(*tab);
}

juce::Colour Tabs::getBorderColour() const
{
    return _borderOverride.value_or(juce::Colours::transparentBlack);
}

void Tabs::setSelectedBorderColour(juce::Colour colour)
{
    _selectedBorderOverride = colour;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetSelectedBorderColour()
{
    _selectedBorderOverride = juce::Colour();
    for (const auto& tab : _tabs) applyStyle(*tab);
}

juce::Colour Tabs::getSelectedBorderColour() const
{
    return _selectedBorderOverride.value_or(getBorderColour());
}

void Tabs::setBorderRadius(float radius)
{
    _borderRadiusOverride = radius;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetBorderRadius()
{
    _borderRadiusOverride = -1.f;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

float Tabs::getBorderRadius() const
{
    return _borderRadiusOverride >= 0.f ? _borderRadiusOverride : 8.f;
}

void Tabs::setFontSize(Theme::FontSize size)
{
    _fontSizeOverride = size;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetFontSize()
{
    _fontSizeOverride = std::nullopt;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

Theme::FontSize Tabs::getFontSize() const
{
    return _fontSizeOverride.value_or(Theme::CAPTION);
}

void Tabs::setFontWeight(Theme::FontWeight weight)
{
    _fontWeightOverride = weight;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetFontWeight()
{
    _fontWeightOverride = std::nullopt;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

Theme::FontWeight Tabs::getFontWeight() const
{
    return _fontWeightOverride.value_or(Theme::REGULAR);
}

void Tabs::setFont(Theme::FontWeight weight, Theme::FontSize size)
{
    _fontWeightOverride = weight;
    _fontSizeOverride = size;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetFont()
{
    _fontWeightOverride = std::nullopt;
    _fontSizeOverride = std::nullopt;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::setHeightType(Theme::HeightType type)
{
    _heightType = type;
    for (const auto& tab : _tabs) applyStyle(*tab);
    resized();
}

void Tabs::setTabDesign(Theme::TabDesign design)
{
    _design = design;
    applyDesignLayout();
    for (const auto& tab : _tabs) applyStyle(*tab);
    resized();
    repaint();
}

void Tabs::setTextColour(juce::Colour colour)
{
    _textColourOverride = colour;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetTextColour()
{
    _textColourOverride = juce::Colour();
    for (const auto& tab : _tabs) applyStyle(*tab);
}

juce::Colour Tabs::getTextColour() const
{
    return _textColourOverride.value_or(Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
}

void Tabs::setSelectedTextColour(juce::Colour colour)
{
    _selectedTextColourOverride = colour;
    for (const auto& tab : _tabs) applyStyle(*tab);
}

void Tabs::resetSelectedTextColour()
{
    _selectedTextColourOverride = juce::Colour();
    for (const auto& tab : _tabs) applyStyle(*tab);
}

juce::Colour Tabs::getSelectedTextColour() const
{
    return _selectedTextColourOverride.value_or(getTextColour());
}

void Tabs::applyStyle(TextButton& tab) const
{
    tab.setBackgroundColour(getBackgroundColour());
    tab.setSelectedBackgroundColour(getSelectedBackgroundColour());
    tab.setBorderColour(getBorderColour());
    tab.setSelectedBorderColour(getSelectedBorderColour());
    tab.setBorderRadius(getBorderRadius());
    tab.setFontWeight(getFontWeight());
    tab.setFontSize(getFontSize());
    tab.setHeightType(getHeightType());
    tab.setTextColour(getTextColour());
    tab.setSelectedTextColour(getSelectedTextColour());
    tab.setTabDesign(getTabDesign());

    // TabDesign::TAB: the selected tab always stays perfectly square; a tab directly touching it
    // gets a concave notch on that near corner instead, coloured like the selected tab, so its
    // shape reads as flowing smoothly out from underneath the selected tab.
    bool hasLeftNotch = false;
    bool hasRightNotch = false;
    if (_design == Theme::TabDesign::TAB && tab.getID() != _selectedTabID)
    {
        const auto it = std::find_if(_tabs.begin(), _tabs.end(), [&tab](const auto& t) { return t->getID() == tab.getID(); });
        if (it != _tabs.end())
        {
            const auto index = std::distance(_tabs.begin(), it);
            if (index > 0 && _tabs[static_cast<std::size_t>(index) - 1]->getID() == _selectedTabID)
                hasLeftNotch = true;
            if (static_cast<std::size_t>(index) + 1 < _tabs.size() && _tabs[static_cast<std::size_t>(index) + 1]->getID() == _selectedTabID)
                hasRightNotch = true;
        }
    }
    tab.setNotches(hasLeftNotch, hasRightNotch, getSelectedBackgroundColour());
}

void Tabs::addOnTabChangedListener(OnTabChangedListener* listener)
{
    _listeners.push_back(listener);
}

void Tabs::removeListener(OnTabChangedListener* listener)
{
    std::erase(_listeners, listener);
}

}
