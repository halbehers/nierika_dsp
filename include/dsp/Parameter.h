#pragma once

#include <utility>
#include <variant>

namespace nierika::dsp
{

class ParameterValueHolder
{
public:
    using ParameterValue = std::variant<float, int, bool>;

    explicit ParameterValueHolder(const ParameterValue value) : _value(value) {}

    template<typename T>
    T get() const
    {
        if (!std::holds_alternative<T>(_value))
            throw std::bad_variant_access();

        return std::get<T>(_value);
    }

    template<typename T>
    T getOr(const T& fallback) const
    {
        if (std::holds_alternative<T>(_value))
            return std::get<T>(_value);

        return fallback;
    }

    template<typename T>
    [[nodiscard]] bool is() const
    {
        return std::holds_alternative<T>(_value);
    }

    [[nodiscard]] const ParameterValue& raw() const { return _value; }

private:
    ParameterValue _value;
};


class IParameter
{
public:
    virtual ~IParameter() = default;
    IParameter() = default;
    IParameter(const IParameter& other) = default;
    IParameter& operator=(const IParameter& other) = default;

    enum Type
    {
        TYPE_FLOAT,
        TYPE_INT,
        TYPE_BOOL
    };

    [[nodiscard]] virtual std::string getID() const = 0;
    [[nodiscard]] virtual Type getType() const = 0;
    [[nodiscard]] virtual std::string getName() const = 0;
    [[nodiscard]] virtual std::string getDisplayName() const = 0;
    [[nodiscard]] virtual std::string getTooltip() const = 0;
    [[nodiscard]] virtual ParameterValueHolder getDefaultValue() const = 0;
    [[nodiscard]] virtual ParameterValueHolder getMinValue() const = 0;
    [[nodiscard]] virtual ParameterValueHolder getMaxValue() const = 0;
    virtual void onChangeFromVariant(ParameterValueHolder& newValue) = 0;
};

template<typename T>
class Parameter final : public IParameter
{
public:
    Parameter(
        std::string id,
        const Type type,
        std::string  name,
        std::string  displayName,
        std::string  tooltip,
        T defaultValue,
        T minValue,
        T maxValue,
        std::function<void(T)> onChange
    ): IParameter(),
        _id(std::move(id)),
       _type(type),
       _name(std::move(name)),
       _displayName(std::move(displayName)),
       _tooltip(std::move(tooltip)),
       _defaultValue(defaultValue),
       _minValue(minValue),
       _maxValue(maxValue),
       _onChange(onChange) {
    }

    Parameter(const Parameter& other) = default;
    Parameter& operator=(const Parameter& other) = default;

    ~Parameter() override = default;

    [[nodiscard]] std::string getID() const override
    {
        return _id;
    }

    [[nodiscard]] Type getType() const override
    {
        return _type;
    }
    [[nodiscard]] std::string getName() const override
    {
        return _name;
    }
    [[nodiscard]] std::string getDisplayName() const override
    {
        return _displayName;
    }
    [[nodiscard]] std::string getTooltip() const override
    {
        return _tooltip;
    }
    [[nodiscard]] ParameterValueHolder getDefaultValue() const override
    {
        return ParameterValueHolder(_defaultValue);
    }
    [[nodiscard]] ParameterValueHolder getMinValue() const override
    {
        return ParameterValueHolder(_minValue);
    }
    [[nodiscard]] ParameterValueHolder getMaxValue() const override
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
        }
    }

private:
    std::string _id;
    Type _type;
    std::string _name;
    std::string _displayName;
    std::string _tooltip;
    T _defaultValue;
    T _minValue;
    T _maxValue;
    std::function<void(T)> _onChange;
};
}
