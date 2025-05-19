#include <cmath>
#include <type_traits>
#include <limits>

namespace nierika::gui::layout
{

template<typename T>
Spacing<T>::Spacing(T _left, T _top, T _right, T _bottom):
    left(_left),
    top(_top),
    right(_right),
    bottom(_bottom)
{
}

template<typename T>
Spacing<T>::Spacing(T horizontal, T vertical):
    left(horizontal),
    top(vertical),
    right(horizontal),
    bottom(vertical)
{
}

template<typename T>
Spacing<T>::Spacing(T spacing):
    left(spacing),
    top(spacing),
    right(spacing),
    bottom(spacing)
{
}

template<typename T>
Spacing<T>::Spacing(const Spacing<T>& other):
    left(other.left),
    top(other.top),
    right(other.right),
    bottom(other.bottom)
{
}

template<typename T>
Spacing<T>& Spacing<T>::operator=(const Spacing<T>& other)
{
    if (this == &other)
    {
        return *this;
    }

    left = other.left;
    top = other.top;
    right = other.right;
    bottom = other.bottom;

    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator+(T value)
{
    left += value;
    top += value;
    right += value;
    bottom += value;
    
    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator+(const Spacing<T>& other)
{
    left += other.left;
    top += other.top;
    right += other.right;
    bottom += other.bottom;
    
    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator-(T value)
{
    left -= value;
    top -= value;
    right -= value;
    bottom -= value;
    
    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator-(const Spacing<T>& other)
{
    left -= other.left;
    top -= other.top;
    right -= other.right;
    bottom -= other.bottom;
    
    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator*(T value)
{
    left *= value;
    top *= value;
    right *= value;
    bottom *= value;
    
    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator*(const Spacing<T>& other)
{
    left *= other.left;
    top *= other.top;
    right *= other.right;
    bottom *= other.bottom;
    
    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator/(T value)
{
    left /= value;
    top /= value;
    right /= value;
    bottom /= value;
    
    return *this;
}

template<typename T>
Spacing<T> Spacing<T>::operator/(const Spacing<T>& other)
{
    left /= other.left;
    top /= other.top;
    right /= other.right;
    bottom /= other.bottom;
    
    return *this;
}

template<typename T>
bool Spacing<T>::operator==(const Spacing<T>& other) const
{
    if constexpr (std::is_floating_point_v<T>)
    {
        const T epsilon = std::numeric_limits<T>::epsilon() * 100;
        return std::fabs(left - other.left) < epsilon &&
               std::fabs(top - other.top) < epsilon &&
               std::fabs(right - other.right) < epsilon &&
               std::fabs(bottom - other.bottom) < epsilon;
    }
    else
    {
        return left == other.left
                && top == other.top
                && right == other.right
                && bottom == other.bottom;
    }
}

template<typename T>
bool Spacing<T>::operator!=(const Spacing<T>& other) const
{
    return !(*this == other);
}

template<typename T>
void Spacing<T>::addLeft(T value)
{
    left += value;
}

template<typename T>
void Spacing<T>::addTop(T value)
{
    top += value;
}

template<typename T>
void Spacing<T>::addRight(T value)
{
    right += value;
}

template<typename T>
void Spacing<T>::addBottom(T value)
{
    bottom += value;
}

template<typename T>
void Spacing<T>::withLeft(T value)
{
    left = value;
}

template<typename T>
void Spacing<T>::withTop(T value)
{
    top = value;
}

template<typename T>
void Spacing<T>::withRight(T value)
{
    right = value;
}

template<typename T>
void Spacing<T>::withBottom(T value)
{
    bottom = value;
}

template<typename T>
void Spacing<T>::substractLeft(T value)
{
    left -= value;
}

template<typename T>
void Spacing<T>::substractTop(T value)
{
    top -= value;
}

template<typename T>
void Spacing<T>::substractRight(T value)
{
    right -= value;
}

template<typename T>
void Spacing<T>::substractBottom(T value)
{
    bottom -= value;
}

template<typename T>
void Spacing<T>::multiplyLeft(T value)
{
    left *= value;
}

template<typename T>
void Spacing<T>::multiplyTop(T value)
{
    top *= value;
}

template<typename T>
void Spacing<T>::multiplyRight(T value)
{
    right *= value;
}

template<typename T>
void Spacing<T>::multiplyBottom(T value)
{
    bottom *= value;
}

template<typename T>
void Spacing<T>::divideLeft(T value)
{
    left /= value;
}

template<typename T>
void Spacing<T>::divideTop(T value)
{
    top /= value;
}

template<typename T>
void Spacing<T>::divideRight(T value)
{
    right /= value;
}

template<typename T>
void Spacing<T>::divideBottom(T value)
{
    bottom /= value;
}

template<typename T>
Spacing<int> Spacing<T>::toInt()
{
    return {static_cast<int>(left), static_cast<int>(top), static_cast<int>(right), static_cast<int>(bottom)};
}

template<typename T>
Spacing<float> Spacing<T>::toFloat()
{
    return {static_cast<float>(left), static_cast<float>(top), static_cast<float>(right), static_cast<float>(bottom)};
}

template<typename T>
Spacing<double> Spacing<T>::toDouble()
{
    return {static_cast<double>(left), static_cast<double>(top), static_cast<double>(right), static_cast<double>(bottom)};
}

template<typename T>
bool Spacing<T>::isEmpty()
{
    if constexpr (std::is_floating_point<T>::value)
    {
        const T epsilon = std::numeric_limits<T>::epsilon() * 100;
        return std::fabs(left) < epsilon &&
               std::fabs(top) < epsilon &&
               std::fabs(right) < epsilon &&
               std::fabs(bottom) < epsilon;
    }
    else
    {
        return left == 0 && top == 0 && right == 0 && bottom == 0;
    }
}

template<typename T>
juce::Rectangle<T> Spacing<T>::computeBounds(juce::Rectangle<T> bounds)
{
    const auto newX = bounds.getX() + static_cast<T>(left);
    const auto newY = bounds.getY() + static_cast<T>(top);
    const auto newWidth = bounds.getWidth() - static_cast<T>(left) - static_cast<T>(right);
    const auto newHeight = bounds.getHeight() - static_cast<T>(top) - static_cast<T>(bottom);

    return juce::Rectangle<T>(newX, newY, newWidth, newHeight);
}

template<typename T>
juce::Rectangle<T> Spacing<T>::computeBounds(const juce::Component& component)
{
    const auto bounds = component.getLocalBounds();
    const juce::Rectangle<T> typedBounds(static_cast<T>(bounds.getX()), static_cast<T>(bounds.getY()), static_cast<T>(bounds.getWidth()), static_cast<T>(bounds.getHeight()));

    return computeBounds(typedBounds);
}

template<typename T>
juce::Rectangle<T> Spacing<T>::computeBounds()
{
    if (_component == nullptr) return juce::Rectangle<T>(0, 0, 0, 0);

    return computeBounds(*_component);
}

template Spacing<int>::Spacing(int _left, int _top, int _right, int _bottom);
template Spacing<float>::Spacing(float _left, float _top, float _right, float _bottom);
template Spacing<double>::Spacing(double _left, double _top, double _right, double _bottom);
template Spacing<int>::Spacing(int horizontal, int vertical);
template Spacing<float>::Spacing(float horizontal, float vertical);
template Spacing<double>::Spacing(double horizontal, double vertical);
template Spacing<int>::Spacing(int Spacing);
template Spacing<float>::Spacing(float Spacing);
template Spacing<double>::Spacing(double Spacing);
template Spacing<int>::Spacing(const Spacing<int>& other);
template Spacing<float>::Spacing(const Spacing<float>& other);
template Spacing<double>::Spacing(const Spacing<double>& other);

template Spacing<int>& Spacing<int>::operator=(const Spacing<int>& other);
template Spacing<float>& Spacing<float>::operator=(const Spacing<float>& other);
template Spacing<double>& Spacing<double>::operator=(const Spacing<double>& other);
template Spacing<int> Spacing<int>::operator+(int value);
template Spacing<float> Spacing<float>::operator+(float value);
template Spacing<double> Spacing<double>::operator+(double value);
template Spacing<int> Spacing<int>::operator+(const Spacing<int>& other);
template Spacing<float> Spacing<float>::operator+(const Spacing<float>& other);
template Spacing<double> Spacing<double>::operator+(const Spacing<double>& other);
template Spacing<int> Spacing<int>::operator-(int value);
template Spacing<float> Spacing<float>::operator-(float value);
template Spacing<double> Spacing<double>::operator-(double value);
template Spacing<int> Spacing<int>::operator-(const Spacing<int>& other);
template Spacing<float> Spacing<float>::operator-(const Spacing<float>& other);
template Spacing<double> Spacing<double>::operator-(const Spacing<double>& other);
template Spacing<int> Spacing<int>::operator*(int value);
template Spacing<float> Spacing<float>::operator*(float value);
template Spacing<double> Spacing<double>::operator*(double value);
template Spacing<int> Spacing<int>::operator*(const Spacing<int>& other);
template Spacing<float> Spacing<float>::operator*(const Spacing<float>& other);
template Spacing<double> Spacing<double>::operator*(const Spacing<double>& other);
template Spacing<int> Spacing<int>::operator/(int value);
template Spacing<float> Spacing<float>::operator/(float value);
template Spacing<double> Spacing<double>::operator/(double value);
template Spacing<int> Spacing<int>::operator/(const Spacing<int>& other);
template Spacing<float> Spacing<float>::operator/(const Spacing<float>& other);
template Spacing<double> Spacing<double>::operator/(const Spacing<double>& other);
template bool Spacing<int>::operator==(const Spacing<int>& other) const;
template bool Spacing<float>::operator==(const Spacing<float>& other) const;
template bool Spacing<double>::operator==(const Spacing<double>& other) const;
template bool Spacing<int>::operator!=(const Spacing<int>& other) const;
template bool Spacing<float>::operator!=(const Spacing<float>& other) const;
template bool Spacing<double>::operator!=(const Spacing<double>& other) const;

template void Spacing<int>::addLeft(int value);
template void Spacing<float>::addLeft(float value);
template void Spacing<double>::addLeft(double value);
template void Spacing<int>::addTop(int value);
template void Spacing<float>::addTop(float value);
template void Spacing<double>::addTop(double value);
template void Spacing<int>::addRight(int value);
template void Spacing<float>::addRight(float value);
template void Spacing<double>::addRight(double value);
template void Spacing<int>::addBottom(int value);
template void Spacing<float>::addBottom(float value);
template void Spacing<double>::addBottom(double value);

template void Spacing<int>::withLeft(int value);
template void Spacing<float>::withLeft(float value);
template void Spacing<double>::withLeft(double value);
template void Spacing<int>::withTop(int value);
template void Spacing<float>::withTop(float value);
template void Spacing<double>::withTop(double value);
template void Spacing<int>::withRight(int value);
template void Spacing<float>::withRight(float value);
template void Spacing<double>::withRight(double value);
template void Spacing<int>::withBottom(int value);
template void Spacing<float>::withBottom(float value);
template void Spacing<double>::withBottom(double value);

template void Spacing<int>::substractLeft(int value);
template void Spacing<float>::substractLeft(float value);
template void Spacing<double>::substractLeft(double value);
template void Spacing<int>::substractTop(int value);
template void Spacing<float>::substractTop(float value);
template void Spacing<double>::substractTop(double value);
template void Spacing<int>::substractRight(int value);
template void Spacing<float>::substractRight(float value);
template void Spacing<double>::substractRight(double value);
template void Spacing<int>::substractBottom(int value);
template void Spacing<float>::substractBottom(float value);
template void Spacing<double>::substractBottom(double value);

template void Spacing<int>::multiplyLeft(int value);
template void Spacing<float>::multiplyLeft(float value);
template void Spacing<double>::multiplyLeft(double value);
template void Spacing<int>::multiplyTop(int value);
template void Spacing<float>::multiplyTop(float value);
template void Spacing<double>::multiplyTop(double value);
template void Spacing<int>::multiplyRight(int value);
template void Spacing<float>::multiplyRight(float value);
template void Spacing<double>::multiplyRight(double value);
template void Spacing<int>::multiplyBottom(int value);
template void Spacing<float>::multiplyBottom(float value);
template void Spacing<double>::multiplyBottom(double value);

template void Spacing<int>::divideLeft(int value);
template void Spacing<float>::divideLeft(float value);
template void Spacing<double>::divideLeft(double value);
template void Spacing<int>::divideTop(int value);
template void Spacing<float>::divideTop(float value);
template void Spacing<double>::divideTop(double value);
template void Spacing<int>::divideRight(int value);
template void Spacing<float>::divideRight(float value);
template void Spacing<double>::divideRight(double value);
template void Spacing<int>::divideBottom(int value);
template void Spacing<float>::divideBottom(float value);
template void Spacing<double>::divideBottom(double value);

template Spacing<int> Spacing<int>::toInt();
template Spacing<int> Spacing<float>::toInt();
template Spacing<int> Spacing<double>::toInt();
template Spacing<float> Spacing<int>::toFloat();
template Spacing<float> Spacing<float>::toFloat();
template Spacing<float> Spacing<double>::toFloat();
template Spacing<double> Spacing<int>::toDouble();
template Spacing<double> Spacing<float>::toDouble();
template Spacing<double> Spacing<double>::toDouble();

template bool Spacing<int>::isEmpty();
template bool Spacing<float>::isEmpty();
template bool Spacing<double>::isEmpty();

template juce::Rectangle<int> Spacing<int>::computeBounds(juce::Rectangle<int> bounds);
template juce::Rectangle<float> Spacing<float>::computeBounds(juce::Rectangle<float> bounds);
template juce::Rectangle<double> Spacing<double>::computeBounds(juce::Rectangle<double> bounds);
template juce::Rectangle<int> Spacing<int>::computeBounds(const juce::Component& component);
template juce::Rectangle<float> Spacing<float>::computeBounds(const juce::Component& component);
template juce::Rectangle<double> Spacing<double>::computeBounds(const juce::Component& component);

}
