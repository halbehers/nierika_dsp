#pragma once
#include <algorithm>

namespace nierika::gui::layout
{

template<typename T = int>
class Distance {
public:
    enum Unit
    {
        PIXEL,
        PERCENTAGE
    };

    explicit Distance(T value, Unit unit = PIXEL, T minValueInPx = static_cast<T>(-1), T maxValueInPx = static_cast<T>(-1)):
        _value(value),
        _unit(unit),
        _minValueInPx(minValueInPx),
        _maxValueInPx(maxValueInPx)
    {
    }

    explicit Distance(T value, T minValueInPx = static_cast<T>(-1), T maxValueInPx = static_cast<T>(-1)):
        _value(value),
        _unit(PIXEL),
        _minValueInPx(minValueInPx),
        _maxValueInPx(maxValueInPx)
    {
    }

    T getValueInPx(T availableSpaceInPx) {
        if (_unit == PIXEL) return clampValue(_value, availableSpaceInPx);

        const T valueInPx = static_cast<int>(_value / 100.f * static_cast<float>(availableSpaceInPx));

        return clampValue(valueInPx, availableSpaceInPx);
    }

private:
    T _value = 0;
    Unit _unit = PIXEL;
    T _minValueInPx = static_cast<T>(-1);
    T _maxValueInPx = static_cast<T>(-1);

    T clampValue(T value, T availableSpaceInPx)
    {
        value = std::min(value, availableSpaceInPx);

        return std::clamp(value, _minValueInPx == -1 ? value : _minValueInPx, _maxValueInPx == -1 ? value : _maxValueInPx);
    }
};

}
