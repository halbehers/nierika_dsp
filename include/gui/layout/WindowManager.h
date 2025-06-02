#pragma once

#include "../Distance.h"

namespace nierika::gui::layout
{

    class WindowManager final : public Component
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

        struct Window {
            std::string identifier;
            std::unique_ptr<Component> component;
            Distance<> width;
            Distance<> height;
            bool visible = false;
            int zOrder = 10;
            bool movable = false;
            bool closeable = true;
            WindowPosition position = WINDOW_CENTERED;
        };

        explicit WindowManager(juce::Component& parentComponent);
        ~WindowManager() override = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void createWindow(const std::string& identifier, std::unique_ptr<Component> component, Distance<> width, Distance<> height, WindowPosition position = WINDOW_CENTERED, int zOrder = 100, bool movable = false, bool closeable = true);
        void createWindow(std::unique_ptr<Component> component, Distance<> width, Distance<> height, WindowPosition position = WINDOW_CENTERED, int zOrder = 100, bool movable = false, bool closeable = false);
        void showWindow(const std::string& identifier);
        void hideWindow(const std::string& identifier);
        bool hasVisibleWindows();
        bool windowExists(const std::string& identifier, bool logNonExistance = false, const std::string& callMethod = "WindowManager::windowExists") const;

    private:
        juce::Component& _parentComponent;
        std::unordered_map<std::string, Window> _windowsByID {};

        void resetVisibility();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WindowManager)
    };

}
