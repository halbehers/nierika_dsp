#pragma once

#include <variant>
#include <stdexcept>

#include "../utils/Logger.h"

namespace nierika::dsp
{

class ParameterValueHolder
{
public:
    using ParameterValue = std::variant<float, int, bool, std::string>;

    ParameterValueHolder(ParameterValue value) : _value(std::move(value)) {}

    template<typename T>
    T get() const
    {
        if (!std::holds_alternative<T>(_value))
            throw std::bad_variant_access();

        return std::get<T>(_value);
    }

    // Optionally, a non-throwing version:
    template<typename T>
    T getOr(const T& fallback) const
    {
        if (std::holds_alternative<T>(_value))
            return std::get<T>(_value);

        return fallback;
    }

    template<typename T>
    bool is() const
    {
        return std::holds_alternative<T>(_value);
    }

    const ParameterValue& raw() const { return _value; }

private:
    ParameterValue _value;
};


class IParameter
{
public:
    virtual ~IParameter() = default;

    virtual std::string getID() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDisplayName() const = 0;
    virtual std::string getTooltip() const = 0;
    virtual ParameterValueHolder getDefaultValue() const = 0;
    virtual ParameterValueHolder getMinValue() const = 0;
    virtual ParameterValueHolder getMaxValue() const = 0;
    virtual void onChangeFromVariant(ParameterValueHolder& newValue) = 0;
};

template<typename T>
class Parameter : public IParameter
{
public:
    Parameter(
        const std::string& id,
        const std::string& name,
        const std::string& displayName,
        const std::string& tooltip,
        T defaultValue,
        T minValue,
        T maxValue,
        std::function<void(T)> onChange
    ):
        _id(id),
        _name(name),
        _displayName(displayName),
        _tooltip(tooltip),
        _defaultValue(defaultValue),
        _minValue(minValue),
        _maxValue(maxValue),
        _onChange(onChange)
    {
    }

    ~Parameter() = default;

    std::string getID() const override
    {
        return _id;
    }
    std::string getName() const override
    {
        return _name;
    }
    std::string getDisplayName() const override
    {
        return _displayName;
    }
    std::string getTooltip() const override
    {
        return _tooltip;
    }
    ParameterValueHolder getDefaultValue() const override
    {
        return ParameterValueHolder(_defaultValue);
    }
    ParameterValueHolder getMinValue() const override
    {
        return ParameterValueHolder(_minValue);
    }
    ParameterValueHolder getMaxValue() const override
    {
        return ParameterValueHolder(_maxValue);
    }
    
    void onChangeFromVariant(ParameterValueHolder& newValue) override
    {
        if (newValue.is<T>())
            _onChange(newValue.get<T>());
        else
        {
            utils::AppLogger::get().error("Type mismatch for parameter " + _id, "Parameter::onChangeFromVariant");
            throw std::bad_variant_access();
        }
    }

private:
    std::string _id;
    std::string _name;
    std::string _displayName;
    std::string _tooltip;
    T _defaultValue;
    T _minValue;
    T _maxValue;
    std::function<void(T)> _onChange;
};
}
