#include "../../include/gui/Component.h"
#include "../../include/gui/TooltipManager.h"

namespace nierika::gui
{

Component::Component(const std::string& identifier, const std::string& name, const std::string& tooltip):
    juce::Component(),
    _tooltip(tooltip)
{
    setComponentID(identifier);
    setName(name);

    if (_tooltip != "")
        TooltipManager::getInstance().registerComponent(*this);
}

Component::~Component()
{
    TooltipManager::getInstance().unregisterComponent(*this);
}

void Component::setTooltip(const std::string& text)
{
    _tooltip = text;
}

const std::string& Component::getTooltip() const
{
    return _tooltip;
}

}
