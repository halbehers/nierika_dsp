#pragma once

namespace nierika::gui::layout
{

template<typename T>
struct Spacing {
    Spacing(T left, T top, T right, T bottom);
    Spacing(T horizontal, T vertical);
    explicit Spacing(T spacing);
    Spacing() = default;

    Spacing<T>& operator=(const Spacing<T>& other);
    Spacing<T> operator+(T value);
    Spacing<T> operator+(const Spacing<T>& other);
    Spacing<T> operator-(T value);
    Spacing<T> operator-(const Spacing<T>& other);
    Spacing<T> operator*(T value);
    Spacing<T> operator*(const Spacing<T>& other);
    Spacing<T> operator/(T value);
    Spacing<T> operator/(const Spacing<T>& other);
    bool operator==(const Spacing<T>& other) const;
    bool operator!=(const Spacing<T>& other) const;
    
    void addLeft(T value);
    void addTop(T value);
    void addRight(T value);
    void addBottom(T value);

    void withLeft(T value);
    void withTop(T value);
    void withRight(T value);
    void withBottom(T value);
    
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
    
    Spacing<int> toInt();
    Spacing<float> toFloat();
    Spacing<double> toDouble();
    
    bool isEmpty();
    
    juce::Rectangle<T> computeBounds(juce::Rectangle<T> bounds);
    juce::Rectangle<T> computeBounds(juce::Component& component);
    juce::Rectangle<T> computeBounds();

    void attachComponent(juce::Component* component) { _component = component; }
    
    T left, top, right, bottom = 0;

    enum Type
    {
        MARGIN,
        PADDING
    };

    void setType(Type type) { _type = type; }
    void setAsMargin() { _type = MARGIN; }
    void setAsPadding() { _type = PADDING; }

private:
    juce::Component* _component = nullptr;
    Type _type = MARGIN;
};

}
