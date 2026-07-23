#include "../../../include/gui/element/PopupPanel.h"
#include "../../../include/gui/laf/PopupPanel.h"

namespace nierika::gui::element
{

namespace
{
    laf::PopupPanel& getPopupPanelLookAndFeel()
    {
        static laf::PopupPanel lookAndFeel;
        return lookAndFeel;
    }
}

PopupPanel::PopupPanel(const std::string& identifier, const char* svgBinary):
    Component(identifier),
    _iconButton(identifier + "-icon", svgBinary)
{
    addAndMakeVisible(_iconButton);
    _iconButton.addOnClickListener(this);
}

PopupPanel::~PopupPanel()
{
    _iconButton.removeListener(this);
}

void PopupPanel::resized()
{
    Component::resized();
    _iconButton.setBounds(getLocalBounds());
}

void PopupPanel::onButtonClick(const std::string& componentID)
{
    if (componentID != _iconButton.getComponentID() || !_contentProvider)
        return;

    auto content = _contentProvider();
    if (content == nullptr)
        return;

    if (_popupWidth >= 0)
        content->setSize(_popupWidth, content->getHeight());

    // Anchoring within the editor's own window (rather than adding the popup to the desktop as
    // its own OS window) keeps positioning sane and avoids cross-window modal/focus glitches.
    auto* parent = getTopLevelComponent();
    const auto area = parent != nullptr ? parent->getLocalArea(this, getLocalBounds()) : getScreenBounds();

    show(std::move(content), area.withY(area.getY() - 16), parent);
}

juce::CallOutBox& PopupPanel::show(std::unique_ptr<juce::Component> content, const juce::Rectangle<int> areaToPointTo, juce::Component* parentComponent)
{
    auto& box = juce::CallOutBox::launchAsynchronously(std::move(content), areaToPointTo, parentComponent);
    box.setLookAndFeel(&getPopupPanelLookAndFeel());
    box.setArrowSize(0.f);
    return box;
}

}
