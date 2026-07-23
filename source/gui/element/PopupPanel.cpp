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

    // Point at the icon's full bounds (not a zero-height sliver pinned to its top edge) so
    // CallOutBox's placement lands the popup just below the icon without overlapping it. An
    // earlier attempt at tightening the gap (pointing at a zero-height rect at the icon's top)
    // made CallOutBox's "below" placement land the popup's top ~12px *above* the icon's top,
    // overlapping the icon's own clickable bounds - since the icon remains in front there
    // (it's nested deeper in the component tree than the top-level-added CallOutBox in z-order
    // terms, but overlapping click regions are exactly the kind of thing that silently eats
    // clicks), every click on the popup in that overlapping region was being re-captured by the
    // icon button instead of reaching the popup's content, which read as "the checkbox isn't
    // clickable". Keep the small, non-overlapping default gap instead.
    show(std::move(content), area, parent);
}

juce::CallOutBox& PopupPanel::show(std::unique_ptr<juce::Component> content, const juce::Rectangle<int> areaToPointTo, juce::Component* parentComponent)
{
    auto& box = juce::CallOutBox::launchAsynchronously(std::move(content), areaToPointTo, parentComponent);
    box.setLookAndFeel(&getPopupPanelLookAndFeel());
    box.setArrowSize(0.f);
    return box;
}

}
