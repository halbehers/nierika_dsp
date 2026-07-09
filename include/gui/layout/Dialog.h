#pragma once

namespace nierika::gui::layout
{
    class Dialog: public Window
    {
    public:
        Dialog(const std::string& identifier, WindowsManager& windowManager);
        ~Dialog() override = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void setTitle(const std::string& title);
        void setText(const std::string& text);

    private:
        element::SVGButton _closeButton { Icons::getPlus() };
        WindowsManager& _windowManager;
        element::Text _title;
        element::Text _text;

        GridLayout<Component> _layout;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dialog)
    };

}
