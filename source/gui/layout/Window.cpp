#include "../../../include/gui/layout/Window.h"

namespace nierika::gui::layout {

    Window::Window(const std::string& identifier, Distance<> width, Distance<> height):
        Component(identifier),
        _width(width),
        _height(height)
    {
        addMouseListener(this, true);
    }

    void Window::resized()
    {
        Component::resized();
        replaceWindow();
    }

    Distance<> Window::getWindowWidth() const
    {
        return _width;
    }

    void Window::setWindowWidth(Distance<> width) {
        _width = width;
    }

    Distance<> Window::getWindowHeight() const
    {
        return _height;
    }

    void Window::setWindowHeight(Distance<> height)
    {
        _height = height;
    }

    int Window::getZOrder() const
    {
        return _zOrder;
    }

    void Window::setZOrder(int zOrder)
    {
        _zOrder = zOrder;
    }

    bool Window::isMovable() const
    {
        return _movable;
    }

    void Window::setMovable(bool movable)
    {
        _movable = movable;
    }

    bool Window::isCloseable() const
    {
        return _closeable;
    }

    void Window::setCloseable(bool closeable)
    {
        _closeable = closeable;
    }

    Window::WindowPosition Window::getWindowPosition() const
    {
        return _position;
    }

    void Window::setWindowPosition(WindowPosition position, int absoluteXPosition, int absoluteYPosition)
    {
        _position = position;
        _absoluteXPosition = absoluteXPosition;
        _absoluteYPosition = absoluteYPosition;
    }

    juce::Rectangle<float> Window::getMovableZone() const
    {
        return juce::Rectangle(0.f, 0.f, static_cast<float>(getWidth()), 40.f);
    }

    void Window::setWindowsManager(WindowsManager* windowsManager)
    {
        _manager = windowsManager;
    }

    void Window::mouseDown(const juce::MouseEvent& event)
    {
        if (getMovableZone().contains(event.position.getX(), event.position.getY()))
        {
            _onDrag = true;
        }
    }

    void Window::mouseDrag(const juce::MouseEvent& event)
    {
        (void) event;

        if (_onDrag)
        {
            const int windowWidthInPx = getWindowWidth().getValueInPx(getWidth());

            setWindowPosition(WINDOW_ABSOLUTE, static_cast<int>(event.position.getX()) + windowWidthInPx / 2, static_cast<int>(event.position.getY()) + 20);
        }
    }

    void Window::mouseUp(const juce::MouseEvent& event)
    {
        (void) event;

        resetDrag();
    }

    bool Window::keyPressed(const juce::KeyPress &key)
    {
        if (key == juce::KeyPress::escapeKey)
        {
            resetDrag();
            return true;
        }

        return false;
    }

    void Window::resetDrag()
    {
        _onDrag = false;
    }

    void Window::replaceWindow()
    {
        if (_manager == nullptr) {
            utils::AppLogger::warn("No manager has been set for window '" + getID() + "'.", "Window::replaceWindow");
            return;
        }

        const int windowWidthInPx = getWindowWidth().getValueInPx(_manager->getWidth());
        const int windowHeightInPx = getWindowHeight().getValueInPx(_manager->getHeight());

        switch (getWindowPosition()) {
            case WINDOW_CENTERED:
                setBounds(juce::Rectangle(_manager->getWidth() / 2 - windowWidthInPx / 2, _manager->getHeight() / 2 - windowHeightInPx / 2, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_TOP_CENTER:
                setBounds(juce::Rectangle(_manager->getWidth() / 2 - windowWidthInPx / 2, 0, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_TOP_LEFT:
                setBounds(juce::Rectangle(0, 0, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_TOP_RIGHT:
                setBounds(juce::Rectangle(_manager->getWidth() - windowWidthInPx, 0, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_BOTTOM_CENTER:
                setBounds(juce::Rectangle(_manager->getWidth() / 2 - windowWidthInPx / 2, _manager->getHeight() - windowHeightInPx, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_BOTTOM_LEFT:
                setBounds(juce::Rectangle(0, _manager->getHeight() - windowHeightInPx, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_BOTTOM_RIGHT:
                setBounds(juce::Rectangle(_manager->getWidth() - windowWidthInPx, _manager->getHeight() - windowHeightInPx, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_CENTER_LEFT:
                setBounds(juce::Rectangle(0, _manager->getHeight() / 2 - windowHeightInPx / 2, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_CENTER_RIGHT:
                setBounds(juce::Rectangle(_manager->getWidth() - windowWidthInPx, _manager->getHeight() / 2 - windowHeightInPx / 2, windowWidthInPx, windowHeightInPx));
                break;
            case WINDOW_ABSOLUTE:
                setBounds(_absoluteXPosition, _absoluteYPosition, windowWidthInPx, windowHeightInPx);
                break;
        }
    }
}