#pragma once

namespace nierika::gui::layout
{
    class WindowsManager;

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
            WINDOW_CENTER_RIGHT,
            WINDOW_ABSOLUTE
        };

        Window(const std::string& identifier, Distance<> width, Distance<> height);
        ~Window() override = default;

        void resized() override;

        Distance<> getWindowWidth() const;
        void setWindowWidth(Distance<> width);
        Distance<> getWindowHeight() const;
        void setWindowHeight(Distance<> height);
        int getZOrder() const;
        void setZOrder(int zOrder);
        bool isMovable() const;
        void setMovable(bool movable);
        bool isCloseable() const;
        void setCloseable(bool closeable);
        WindowPosition getWindowPosition() const;
        void setWindowPosition(WindowPosition position, int absoluteXPosition = -1, int absoluteYPosition = -1);

        void mouseDown(const juce::MouseEvent& event) override;
        void mouseDrag(const juce::MouseEvent& event) override;
        void mouseUp(const juce::MouseEvent& event) override;
        bool keyPressed(const juce::KeyPress &key) override;

        void replaceWindow();

        void setWindowsManager(WindowsManager* windowsManager);

    private:
        Distance<> _width;
        Distance<> _height;
        int _zOrder = 10;
        bool _movable = false;
        bool _closeable = true;
        WindowPosition _position = WINDOW_CENTERED;
        bool _onDrag = false;
        int _absoluteXPosition = -1;
        int _absoluteYPosition = -1;

        WindowsManager* _manager = nullptr;

        juce::Rectangle<float> getMovableZone() const;
        void resetDrag();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Window)
    };

}
