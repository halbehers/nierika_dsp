#pragma once

#include <functional>
#include <memory>

#include "SVGButton.h"

namespace nierika::gui::element
{

// An icon button (mirrors SVGButton's own icon API) that, on click, shows a small anchored popup
// - built on juce::CallOutBox, styled to match this module's combo box dropdown popup (same
// corner radius, background/border, no native drop shadow, no arrow tail) - with content supplied
// fresh on every click via setContentProvider(). Also exposes the underlying show(...) launcher
// directly, for a popup not anchored to an icon button.
class PopupPanel: public Component, public SVGButton::OnClickListener
{
public:
    PopupPanel(const std::string& identifier, const char* svgBinary);
    ~PopupPanel() override;

    void resized() override;

    void setIconBinary(const char* svgBinary) { _iconButton.setIconBinary(svgBinary); }
    const char* getIconBinary() const { return _iconButton.getIconBinary(); }

    void setIconSize(float size) { _iconButton.setIconSize(size); }
    void resetIconSize() { _iconButton.resetIconSize(); }
    float getIconSize() const { return _iconButton.getIconSize(); }

    // Called on click to build fresh popup content (rebuilt every click, so it can reflect live
    // state). Returning nullptr from the provider (or leaving it unset) makes a click a no-op.
    void setContentProvider(std::function<std::unique_ptr<juce::Component>()> provider) { _contentProvider = std::move(provider); }

    // Width the popup content is resized to before being shown (its height is left untouched -
    // content is expected to size itself vertically, e.g. by row count). -1 (default) leaves
    // whatever width the content set for itself. Named getPopupWidth(), not getWidth(), to avoid
    // shadowing juce::Component::getWidth() (this component's own on-screen width, unrelated).
    void setWidth(int width) { _popupWidth = width; }
    int getPopupWidth() const { return _popupWidth; }

    // content must already have its size set (CallOutBox's own contract). areaToPointTo is in
    // parentComponent's coordinates if given, else global screen coordinates (parentComponent =
    // nullptr adds the popup straight to the desktop - prefer passing one, see onButtonClick).
    // Returns the live CallOutBox - per its own documented lifetime contract, don't hold a raw
    // pointer/reference to it past the current call.
    static juce::CallOutBox& show(std::unique_ptr<juce::Component> content, juce::Rectangle<int> areaToPointTo, juce::Component* parentComponent = nullptr);

private:
    void onButtonClick(const std::string& componentID) override;

    SVGButton _iconButton;
    std::function<std::unique_ptr<juce::Component>()> _contentProvider;
    int _popupWidth = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupPanel)
};

}
