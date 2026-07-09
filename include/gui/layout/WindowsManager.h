#pragma once

#include "../Distance.h"
#include "Window.h"

namespace nierika::gui::layout
{

    class WindowsManager final : public Component
    {
    public:
        explicit WindowsManager(juce::Component& parentComponent);
        ~WindowsManager() override = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void createWindow(std::unique_ptr<Window> window);
        void showWindow(const std::string& identifier);
        void hideWindow(const std::string& identifier);
        bool hasVisibleWindows();
        bool windowExists(const std::string& identifier, bool logNonExistance = false, const std::string& callMethod = "WindowManager::windowExists") const;

    private:
        juce::Component& _parentComponent;
        std::unordered_map<std::string, std::unique_ptr<Window>> _windowsByID {};

        void resetVisibility();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WindowsManager)
    };

}
