#pragma once

#include <functional>
#include <memory>

#include "SVGButton.h"

namespace nierika::gui::element
{

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

    void setContentProvider(std::function<std::unique_ptr<juce::Component>()> provider) { _contentProvider = std::move(provider); }

    void setWidth(int width) { _popupWidth = width; }
    int getPopupWidth() const { return _popupWidth; }

    static juce::CallOutBox& show(std::unique_ptr<juce::Component> content, juce::Rectangle<int> areaToPointTo, juce::Component* parentComponent = nullptr);

private:
    void onButtonClick(const std::string& componentID) override;

    SVGButton _iconButton;
    std::function<std::unique_ptr<juce::Component>()> _contentProvider;
    int _popupWidth = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupPanel)
};

}
