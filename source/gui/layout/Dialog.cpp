#include "../../../include/gui/layout/Dialog.h"

namespace nierika::gui::layout {

    Dialog::Dialog(const std::string& identifier, WindowsManager& windowManager):
        Window(identifier, Distance(70, Distance<>::PERCENTAGE, 400), Distance(20, Distance<>::PERCENTAGE, 40)),
        _windowManager(windowManager),
        _title("dialog-title"),
        _text("dialog-text"),
        _layout(*this)
    {
        setPadding(12.f, 12.f);
        displayBorder(Theme::ThemeColor::LIGHT_SHADE, 1.f, 16.f);
        displayBackground(Theme::newColor(Theme::ThemeColor::PRIMARY).asJuce(), 16.f);

        _closeButton.onClick = [this] {
            _windowManager.hideWindow(getID());
        };

        _title.setFontSize(Theme::HEADING);
        _title.setJustificationType(juce::Justification::topLeft);
        _text.setFontSize(Theme::CAPTION);
        _title.setJustificationType(juce::Justification::centred);
        _title.displayBackground(Theme::ThemeColor::ACCENT);

        _layout.setDisplayGrid(true);

        _layout.init({ 1, 7 }, { 30, 1 });
        if (isCloseable())
            _layout.addComponent("close", _closeButton, 0, 1, 1, 1);
        _layout.addComponent("title", _title, 0, 0, 1, 1);
        _layout.addComponent("text", _text, 1, 0, 1, 1);
    }

    void Dialog::paint(juce::Graphics& g)
    {
        Component::paint(g);

        _layout.paint(g);
    }

    void Dialog::resized()
    {
        Window::resized();
        _layout.resized();
    }

    void Dialog::setTitle(const std::string& title) {
        _title.setText(title);
        repaint();
    }

    void Dialog::setText(const std::string& text)
    {
        _text.setText(text);
        repaint();
    }
}