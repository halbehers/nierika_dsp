#include "../../../include/gui/layout/WindowManager.h"

namespace nierika::gui::layout {

    WindowManager::WindowManager(juce::Component& parentComponent):
        _parentComponent(parentComponent)
    {
        parentComponent.addAndMakeVisible(this, 1000);
    }

    void WindowManager::paint(juce::Graphics& g)
    {
        Component::paint(g);
    }

    void WindowManager::resized()
    {
        Component::resized();
        for (auto & [_, window] : _windowsByID)
        {
            const int windowWidthInPx = window.width.getValueInPx(getWidth());
            const int windowHeightInPx = window.height.getValueInPx(getHeight());

            switch (window.position) {
                case WINDOW_CENTERED:
                    window.component->setBounds(juce::Rectangle(getWidth() / 2 - windowWidthInPx / 2, getHeight() / 2 - windowHeightInPx / 2, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_TOP_CENTER:
                    window.component->setBounds(juce::Rectangle(getWidth() / 2 - windowWidthInPx / 2, 0, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_TOP_LEFT:
                    window.component->setBounds(juce::Rectangle(0, 0, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_TOP_RIGHT:
                    window.component->setBounds(juce::Rectangle(getWidth() - windowWidthInPx, 0, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_BOTTOM_CENTER:
                    window.component->setBounds(juce::Rectangle(getWidth() / 2 - windowWidthInPx / 2, getHeight() - windowHeightInPx, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_BOTTOM_LEFT:
                    window.component->setBounds(juce::Rectangle(0, getHeight() - windowHeightInPx, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_BOTTOM_RIGHT:
                    window.component->setBounds(juce::Rectangle(getWidth() - windowWidthInPx, getHeight() - windowHeightInPx, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_CENTER_LEFT:
                    window.component->setBounds(juce::Rectangle(0, getHeight() / 2 - windowHeightInPx / 2, windowWidthInPx, windowHeightInPx));
                    break;
                case WINDOW_CENTER_RIGHT:
                    window.component->setBounds(juce::Rectangle(getWidth() - windowWidthInPx, getHeight() / 2 - windowHeightInPx / 2, windowWidthInPx, windowHeightInPx));
                    break;
            }
        }
    }

    void WindowManager::createWindow(const std::string& identifier, std::unique_ptr<Component> component, Distance<> width, Distance<> height, WindowPosition position, int zOrder, bool movable, bool closeable)
    {
        addChildComponent(component.get(), 1000);
        _windowsByID.insert(
            std::make_pair(
                identifier,
                Window({.identifier = identifier,
                       .component = std::move(component),
                       .width = width,
                       .height = height,
                       .position = position,
                       .zOrder = zOrder,
                       .movable = movable,
                       .closeable = closeable})
            )
        );
    }

    void WindowManager::createWindow(std::unique_ptr<Component> component, Distance<> width, Distance<> height, WindowPosition position, int zOrder, bool movable, bool closeable)
    {
        createWindow(component->getID(), std::move(component), width, height, position, zOrder, movable, closeable);
    }

    void WindowManager::showWindow(const std::string& identifier)
    {
        if (!windowExists(identifier, true, "WindowManager::showWindow")) return;

        Window& window = _windowsByID.at(identifier);

        window.visible = true;
        window.component->setVisible(true);

        resetVisibility();
    }

    void WindowManager::hideWindow(const std::string& identifier)
    {
        if (!windowExists(identifier, true, "WindowManager::hideWindow")) return;

        Window& window = _windowsByID.at(identifier);

        window.visible = false;
        window.component->setVisible(false);

        resetVisibility();
    }

    bool WindowManager::windowExists(const std::string& identifier, bool logNonExistance, const std::string& callMethod) const {
        if (!_windowsByID.contains(identifier)) {
            if (logNonExistance)
                utils::AppLogger::warn("Window '" + identifier + "' not found.", callMethod);
            return false;
        }
        return true;
    }

    bool WindowManager::hasVisibleWindows()
    {
        for (const auto& [_, window] : _windowsByID) {
            if (window.visible) return true;
        }
        return false;
    }

    void WindowManager::resetVisibility()
    {
        setVisible(hasVisibleWindows());
    }
}