#pragma once

namespace nierika::gui::layout
{
    class Dialog: public Component
    {
    public:
        Dialog(const std::string& identifier, WindowManager& windowManager, const std::string& text = "");
        ~Dialog() override = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void setText(const std::string& text);

    private:
        element::SVGButton _closeButton { Icons::getPlus() };
        WindowManager& _windowManager;
        std::string _text;

        GridLayout<Component> _layout;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Dialog)
    };

}
