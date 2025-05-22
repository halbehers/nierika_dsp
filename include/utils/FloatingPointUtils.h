#pragma once

#include <cmath>

namespace nierika::utils
{
    class FloatingPointUtils
    {
    public:
        template<typename T>
        static T round(T value, int precision = 2) {
            const auto multiplier = static_cast<T>(std::pow(10.f, precision));
            return std::ceil(value * multiplier) / multiplier;
        }

        template<typename T>
        static bool isEmpty(T value) {
            const T epsilon = std::numeric_limits<T>::epsilon() * 100;
            return std::fabs(value) < epsilon;
        }
    };
}