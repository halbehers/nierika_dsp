#pragma once

#include <cmath>

namespace nierika::utils
{
    class FloatingPointUtils
    {
    public:
        template<typename T>
        static T round(T value, int precision = 2)
        {
            static_assert(std::is_floating_point_v<T>, "round requires a floating-point type");

            const auto multiplier = static_cast<T>(std::pow(10.f, precision));
            return std::ceil(value * multiplier) / multiplier;
        }

        template<typename T>
        static bool isEmpty(T value)
        {
            static_assert(std::is_floating_point_v<T>, "isEmpty requires a floating-point type");

            T threshold;
            if constexpr (std::is_same_v<T, float>)
                threshold = 1e-6f;
            else if constexpr (std::is_same_v<T, double>)
                threshold = 2e-3;
            else
                threshold = 1e-14; // long double or fallback

            return std::fabs(value) < threshold;
        }

        template<typename T>
        static bool isPositive(T value)
        {
            static_assert(std::is_floating_point_v<T>, "isPositive requires a floating-point type");

            return !isEmpty(value) && value > 0;
        }

        template<typename T>
        static bool isNegative(T value)
        {
            static_assert(std::is_floating_point_v<T>, "isNegative requires a floating-point type");

            return !isEmpty(value) && value < 0;
        }


        template<typename T>
        static bool isEqual(T a, T b)
        {
            static_assert(std::is_floating_point_v<T>, "isEqual requires a floating-point type");

            T threshold;
            if constexpr (std::is_same_v<T, float>)
                threshold = 1e-6f;
            else if constexpr (std::is_same_v<T, double>)
                threshold = 2e-3;
            else
                threshold = 1e-14; // long double or fallback

            return std::fabs(a - b) < threshold;
        }

        template <typename T>
        static T mapRange(T value, T inMin, T inMax, T outMin, T outMax)
        {
            if (isEqual(inMin, inMax))
                throw std::invalid_argument("Input range cannot be zero.");

            T normalized = (value - inMin) / (inMax - inMin);

            return outMin + normalized * (outMax - outMin);
        }
    };
}