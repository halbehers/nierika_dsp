#include "../../../include/gui/layout/Dialog.h"

namespace nierika::gui::layout {

    Dialog::Dialog(const std::string& identifier, WindowsManager& windowManager, const std::string& text):
        Component(identifier),
        _windowManager(windowManager),
        _text(text),
        _layout(*this)
    {
        setPadding(12.f, 12.f);
        displayBorder(Theme::ThemeColor::LIGHT_SHADE, 1.f, 16.f);
        displayBackground(Theme::newColor(Theme::ThemeColor::PRIMARY).asJuce(), 16.f);

        _closeButton.onClick = [this] {
            _windowManager.hideWindow(getID());
        };

        _layout.init({ 1, 7 }, { 30, 1 });
        _layout.addComponent("close", _closeButton, 0, 1, 1, 1);
    }

    void Dialog::paint(juce::Graphics& g)
    {
        Component::paint(g);

        _layout.paint(g);

        g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());
        g.setFont(Theme::newFont(Theme::REGULAR, Theme::CAPTION));
        g.drawFittedText(_text, getLocalBounds(), juce::Justification::centred, 2);
    }

    void Dialog::resized()
    {
        Component::resized();
        _layout.resized();
    }

    void Dialog::setText(const std::string& text)
    {
        _text = text;
        repaint();
    }
}