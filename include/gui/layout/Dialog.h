#pragma once

namespace nierika::gui::layout
{
    class Dialog: public Window, public element::SVGButton::OnClickListener
    {
    public:
        Dialog(const std::string& identifier, WindowsManager& windowManager);
        ~Dialog() override;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void setTitle(const std::string& title);
        void setText(const std::string& text);

        void onButtonClick(const std::string& componentID) override;

    private:
        element::SVGButton _closeButton { "close-button", Icons::getPlus() };
        WindowsManager& _windowManager;
        element::Text _title;
        element::Text _text;

        GridLayout<Component> _layout;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dialog)
    };

}
