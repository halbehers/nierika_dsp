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
    setMargin(12.f, 4.f, 12.f, 8.f);
    _layout.setGap(8.f);
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
    
    button->setColour(juce::TextButton::ColourIds::buttonColourId, Theme::ThemeColor::LIGHT_SHADE);

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

void Tabs::addOnTabChangedListener(OnTabChangedListener* listener)
{
    _listeners.push_back(listener);
}

void Tabs::removeListener(OnTabChangedListener* listener)
{
    std::erase(_listeners, listener);
}

}
