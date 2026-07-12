#include "../../../include/gui/layout/WindowsManager.h"

namespace nierika::gui::layout {

    WindowsManager::WindowsManager(juce::Component& parentComponent):
        _parentComponent(parentComponent)
    {
        parentComponent.addAndMakeVisible(this, 1000);

        // addAndMakeVisible leaves this visible (JUCE components default to visible), which - at
        // the topmost z-order and sized to cover the whole parent once setBounds() runs - would
        // silently swallow every mouse click underneath it until the first showWindow/hideWindow
        // call happens to run resetVisibility(). Start hidden instead: correct from construction,
        // with no visible windows yet.
        setVisible(false);
    }

    void WindowsManager::paint(juce::Graphics& g)
    {
        Component::paint(g);
    }

    void WindowsManager::resized()
    {
        Component::resized();
        for (auto & [_, window] : _windowsByID)
        {
            window->resized();
        }
    }

    void WindowsManager::createWindow(std::unique_ptr<Window> window)
    {
        addChildComponent(window.get(), 1000);
        window->setWindowsManager(this);
        _windowsByID.insert(std::make_pair(window->getID(), std::move(window)));
    }

    void WindowsManager::showWindow(const std::string& identifier)
    {
        if (!windowExists(identifier, true, "WindowManager::showWindow")) return;

        Window* window = _windowsByID.at(identifier).get();

        window->setVisible(true);

        resetVisibility();
    }

    void WindowsManager::hideWindow(const std::string& identifier)
    {
        if (!windowExists(identifier, true, "WindowManager::hideWindow")) return;

        Window* window = _windowsByID.at(identifier).get();

        window->setVisible(false);

        resetVisibility();
    }

    bool WindowsManager::windowExists(const std::string& identifier, bool logNonExistance, const std::string& callMethod) const {
        if (!_windowsByID.contains(identifier)) {
            if (logNonExistance)
                utils::AppLogger::warn("Window '" + identifier + "' not found.", callMethod);
            return false;
        }
        return true;
    }

    bool WindowsManager::hasVisibleWindows()
    {
        for (const auto& [_, window] : _windowsByID) {
            if (window->isVisible()) return true;
        }
        return false;
    }

    void WindowsManager::resetVisibility()
    {
        setVisible(hasVisibleWindows());
    }
}