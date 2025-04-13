#pragma once

namespace nierika::gui::layout
{

template<typename T>
struct Margin {
    Margin(T left, T top, T right, T bottom);
    Margin(T horizontal, T vertical);
    Margin(T margin);

    Margin<T> operator=(const Margin<T>& other);
    Margin<T> operator+(T value);
    Margin<T> operator+(const Margin<T>& other);
    Margin<T> operator-(T value);
    Margin<T> operator-(const Margin<T>& other);
    Margin<T> operator*(T value);
    Margin<T> operator*(const Margin<T>& other);
    Margin<T> operator/(T value);
    Margin<T> operator/(const Margin<T>& other);
    bool operator==(const Margin<T>& other) const;
    bool operator!=(const Margin<T>& other) const;
    
    void addLeft(T value);
    void addTop(T value);
    void addRight(T value);
    void addBottom(T value);
    
    void substractLeft(T value);
    void substractTop(T value);
    void substractRight(T value);
    void substractBottom(T value);
    
    void multiplyLeft(T value);
    void multiplyTop(T value);
    void multiplyRight(T value);
    void multiplyBottom(T value);
    
    void divideLeft(T value);
    void divideTop(T value);
    void divideRight(T value);
    void divideBottom(T value);
    
    Margin<int> toInt();
    Margin<float> toFloat();
    Margin<double> toDouble();
    
    bool isEmpty();
    
    juce::Rectangle<T> computeBounds(juce::Rectangle<T> bounds);
    
    T left, top, right, bottom;
};

}
