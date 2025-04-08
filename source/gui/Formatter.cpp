#include <iostream>
#include <sstream>
#include <iomanip>

#include "../../includes/gui/Formatter.h"

namespace ui
{

Formatter::Formatter()
{
}

Formatter::~Formatter()
{
}

std::string Formatter::formatFrequency(float frequency)
{
    std::ostringstream oss;
    if (frequency >= 1000.0f) {
        oss << std::fixed << std::setprecision(1) << (frequency / 1000.0f) << " kHz";
    } else {
        oss << std::fixed << std::setprecision(1) << frequency << " Hz";
    }
    return oss.str();
}

std::string Formatter::toUpper(const std::string& text)
{
    std::string s(text);
    for (int i = 0; i < s.length(); i++)
            s[i] = toupper(s[i]);
    return s;
}

std::string Formatter::formatPercentage(float percentage)
{
    float finalPercentage = percentage;
    if (percentage > 100.0) finalPercentage = 100.0;
    else if (percentage < 0.0) finalPercentage = 0.0;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << " %";
    return oss.str();
}

std::string Formatter::formatPercentage(int percentage)
{
    float finalPercentage = percentage;
    if (percentage > 100) finalPercentage = 100;
    else if (percentage < 0) finalPercentage = 0;
    
    std::ostringstream oss;
    oss << percentage << " %";
    return oss.str();
}

std::string Formatter::formatTimeInMs(float timeInMs)
{
    std::ostringstream oss;
    if (timeInMs >= 1000.0f) {
        oss << std::fixed << std::setprecision(1) << (timeInMs / 1000.0f) << " s";
    } else {
        oss << std::fixed << std::setprecision(1) << timeInMs << " ms";
    }
    return oss.str();
}

}
