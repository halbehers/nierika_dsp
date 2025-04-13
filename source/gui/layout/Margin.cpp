#include "../../../include/gui/layout/Margin.h"

namespace nierika::gui::layout
{

template<typename T>
Margin<T>::Margin(T left, T top, T right, T bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}

template<typename T>
Margin<T>::Margin(T horizontal, T vertical)
{
    this->left = horizontal;
    this->top = vertical;
    this->right = horizontal;
    this->bottom = vertical;
}

template<typename T>
Margin<T>::Margin(T margin)
{
    this->left = margin;
    this->top = margin;
    this->right = margin;
    this->bottom = margin;
}

template<typename T>
Margin<T> Margin<T>::operator=(const Margin<T>& other)
{
    left = other.left;
    top = other.top;
    right = other.right;
    bottom = other.bottom;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator+(T value)
{
    left += value;
    top += value;
    right += value;
    bottom += value;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator+(const Margin<T>& other)
{
    left += other.left;
    top += other.top;
    right += other.right;
    bottom += other.bottom;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator-(T value)
{
    left -= value;
    top -= value;
    right -= value;
    bottom -= value;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator-(const Margin<T>& other)
{
    left -= other.left;
    top -= other.top;
    right -= other.right;
    bottom -= other.bottom;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator*(T value)
{
    left *= value;
    top *= value;
    right *= value;
    bottom *= value;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator*(const Margin<T>& other)
{
    left *= other.left;
    top *= other.top;
    right *= other.right;
    bottom *= other.bottom;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator/(T value)
{
    left /= value;
    top /= value;
    right /= value;
    bottom /= value;
    
    return *this;
}

template<typename T>
Margin<T> Margin<T>::operator/(const Margin<T>& other)
{
    left /= other.left;
    top /= other.top;
    right /= other.right;
    bottom /= other.bottom;
    
    return *this;
}

template<typename T>
bool Margin<T>::operator==(const Margin<T>& other) const
{
    return left == other.left && top == other.top
            && right == other.right && bottom == other.bottom;
}

template<typename T>
bool Margin<T>::operator!=(const Margin<T>& other) const
{
    return !(*this == other);
}

template<typename T>
void Margin<T>::addLeft(T value)
{
    left += value;
}

template<typename T>
void Margin<T>::addTop(T value)
{
    top += value;
}

template<typename T>
void Margin<T>::addRight(T value)
{
    right += value;
}

template<typename T>
void Margin<T>::addBottom(T value)
{
    bottom += value;
}

template<typename T>
void Margin<T>::substractLeft(T value)
{
    left -= value;
}

template<typename T>
void Margin<T>::substractTop(T value)
{
    top -= value;
}

template<typename T>
void Margin<T>::substractRight(T value)
{
    right -= value;
}

template<typename T>
void Margin<T>::substractBottom(T value)
{
    bottom -= value;
}

template<typename T>
void Margin<T>::multiplyLeft(T value)
{
    left *= value;
}

template<typename T>
void Margin<T>::multiplyTop(T value)
{
    top *= value;
}

template<typename T>
void Margin<T>::multiplyRight(T value)
{
    right *= value;
}

template<typename T>
void Margin<T>::multiplyBottom(T value)
{
    bottom *= value;
}

template<typename T>
void Margin<T>::divideLeft(T value)
{
    left /= value;
}

template<typename T>
void Margin<T>::divideTop(T value)
{
    top /= value;
}

template<typename T>
void Margin<T>::divideRight(T value)
{
    right /= value;
}

template<typename T>
void Margin<T>::divideBottom(T value)
{
    bottom /= value;
}

template<typename T>
Margin<int> Margin<T>::toInt()
{
    return Margin<int>(static_cast<int>(left), static_cast<int>(top), static_cast<int>(right), static_cast<int>(bottom));
}

template<typename T>
Margin<float> Margin<T>::toFloat()
{
    return Margin<float>(static_cast<float>(left), static_cast<float>(top), static_cast<float>(right), static_cast<float>(bottom));
}

template<typename T>
Margin<double> Margin<T>::toDouble()
{
    return Margin<double>(static_cast<double>(left), static_cast<double>(top), static_cast<double>(right), static_cast<double>(bottom));
}

template<typename T>
bool Margin<T>::isEmpty()
{
    return left == 0 && top == 0 && right == 0 && bottom == 0;
}

template<typename T>
juce::Rectangle<T> Margin<T>::computeBounds(juce::Rectangle<T> bounds)
{
    const auto newX = bounds.getX() + static_cast<T>(left);
    const auto newY = bounds.getY() + static_cast<T>(top);
    const auto newWidth = bounds.getWidth() - static_cast<T>(left) - static_cast<T>(right);
    const auto newHeight = bounds.getHeight() - static_cast<T>(top) - static_cast<T>(bottom);
    
    return juce::Rectangle<T>(newX, newY, newWidth, newHeight);
}


template Margin<int>::Margin(int left, int top, int right, int bottom);
template Margin<float>::Margin(float left, float top, float right, float bottom);
template Margin<double>::Margin(double left, double top, double right, double bottom);
template Margin<int>::Margin(int horizontal, int vertical);
template Margin<float>::Margin(float horizontal, float vertical);
template Margin<double>::Margin(double horizontal, double vertical);
template Margin<int>::Margin(int margin);
template Margin<float>::Margin(float margin);
template Margin<double>::Margin(double margin);

template Margin<int> Margin<int>::operator=(const Margin<int>& other);
template Margin<float> Margin<float>::operator=(const Margin<float>& other);
template Margin<double> Margin<double>::operator=(const Margin<double>& other);
template Margin<int> Margin<int>::operator+(int value);
template Margin<float> Margin<float>::operator+(float value);
template Margin<double> Margin<double>::operator+(double value);
template Margin<int> Margin<int>::operator+(const Margin<int>& other);
template Margin<float> Margin<float>::operator+(const Margin<float>& other);
template Margin<double> Margin<double>::operator+(const Margin<double>& other);
template Margin<int> Margin<int>::operator-(int value);
template Margin<float> Margin<float>::operator-(float value);
template Margin<double> Margin<double>::operator-(double value);
template Margin<int> Margin<int>::operator-(const Margin<int>& other);
template Margin<float> Margin<float>::operator-(const Margin<float>& other);
template Margin<double> Margin<double>::operator-(const Margin<double>& other);
template Margin<int> Margin<int>::operator*(int value);
template Margin<float> Margin<float>::operator*(float value);
template Margin<double> Margin<double>::operator*(double value);
template Margin<int> Margin<int>::operator*(const Margin<int>& other);
template Margin<float> Margin<float>::operator*(const Margin<float>& other);
template Margin<double> Margin<double>::operator*(const Margin<double>& other);
template Margin<int> Margin<int>::operator/(int value);
template Margin<float> Margin<float>::operator/(float value);
template Margin<double> Margin<double>::operator/(double value);
template Margin<int> Margin<int>::operator/(const Margin<int>& other);
template Margin<float> Margin<float>::operator/(const Margin<float>& other);
template Margin<double> Margin<double>::operator/(const Margin<double>& other);
template bool Margin<int>::operator==(const Margin<int>& other) const;
template bool Margin<float>::operator==(const Margin<float>& other) const;
template bool Margin<double>::operator==(const Margin<double>& other) const;
template bool Margin<int>::operator!=(const Margin<int>& other) const;
template bool Margin<float>::operator!=(const Margin<float>& other) const;
template bool Margin<double>::operator!=(const Margin<double>& other) const;

template void Margin<int>::addLeft(int value);
template void Margin<float>::addLeft(float value);
template void Margin<double>::addLeft(double value);
template void Margin<int>::addTop(int value);
template void Margin<float>::addTop(float value);
template void Margin<double>::addTop(double value);
template void Margin<int>::addRight(int value);
template void Margin<float>::addRight(float value);
template void Margin<double>::addRight(double value);
template void Margin<int>::addBottom(int value);
template void Margin<float>::addBottom(float value);
template void Margin<double>::addBottom(double value);

template void Margin<int>::substractLeft(int value);
template void Margin<float>::substractLeft(float value);
template void Margin<double>::substractLeft(double value);
template void Margin<int>::substractTop(int value);
template void Margin<float>::substractTop(float value);
template void Margin<double>::substractTop(double value);
template void Margin<int>::substractRight(int value);
template void Margin<float>::substractRight(float value);
template void Margin<double>::substractRight(double value);
template void Margin<int>::substractBottom(int value);
template void Margin<float>::substractBottom(float value);
template void Margin<double>::substractBottom(double value);

template void Margin<int>::multiplyLeft(int value);
template void Margin<float>::multiplyLeft(float value);
template void Margin<double>::multiplyLeft(double value);
template void Margin<int>::multiplyTop(int value);
template void Margin<float>::multiplyTop(float value);
template void Margin<double>::multiplyTop(double value);
template void Margin<int>::multiplyRight(int value);
template void Margin<float>::multiplyRight(float value);
template void Margin<double>::multiplyRight(double value);
template void Margin<int>::multiplyBottom(int value);
template void Margin<float>::multiplyBottom(float value);
template void Margin<double>::multiplyBottom(double value);

template void Margin<int>::divideLeft(int value);
template void Margin<float>::divideLeft(float value);
template void Margin<double>::divideLeft(double value);
template void Margin<int>::divideTop(int value);
template void Margin<float>::divideTop(float value);
template void Margin<double>::divideTop(double value);
template void Margin<int>::divideRight(int value);
template void Margin<float>::divideRight(float value);
template void Margin<double>::divideRight(double value);
template void Margin<int>::divideBottom(int value);
template void Margin<float>::divideBottom(float value);
template void Margin<double>::divideBottom(double value);

template Margin<int> Margin<int>::toInt();
template Margin<int> Margin<float>::toInt();
template Margin<int> Margin<double>::toInt();
template Margin<float> Margin<int>::toFloat();
template Margin<float> Margin<float>::toFloat();
template Margin<float> Margin<double>::toFloat();
template Margin<double> Margin<int>::toDouble();
template Margin<double> Margin<float>::toDouble();
template Margin<double> Margin<double>::toDouble();

template bool Margin<int>::isEmpty();
template bool Margin<float>::isEmpty();
template bool Margin<double>::isEmpty();

template juce::Rectangle<int> Margin<int>::computeBounds(juce::Rectangle<int> bounds);
template juce::Rectangle<float> Margin<float>::computeBounds(juce::Rectangle<float> bounds);
template juce::Rectangle<double> Margin<double>::computeBounds(juce::Rectangle<double> bounds);

}
