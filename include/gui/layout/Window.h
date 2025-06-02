#pragma once

namespace nierika::gui::layout
{
    class Window: public Component
    {
    public:
        enum WindowPosition {
            WINDOW_CENTERED,
            WINDOW_TOP_CENTER,
            WINDOW_TOP_LEFT,
            WINDOW_TOP_RIGHT,
            WINDOW_BOTTOM_CENTER,
            WINDOW_BOTTOM_LEFT,
            WINDOW_BOTTOM_RIGHT,
            WINDOW_CENTER_LEFT,
            WINDOW_CENTER_RIGHT
        };

        Window();
        ~Window() override = default;

        std::string identifier;
        std::unique_ptr<Component> component;
        Distance<> width;
        Distance<> height;
        bool visible = false;
        int zOrder = 10;
        bool movable = false;
        bool closeable = true;
        WindowPosition position = WINDOW_CENTERED;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Window)
    };

}
