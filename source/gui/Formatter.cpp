#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

#include "../../include/gui/Formatter.h"

namespace nierika::gui
{

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
    for (std::size_t i = 0; i < s.length(); ++i)
    {
        const auto ch = static_cast<unsigned char>(s[i]);
        s[i] = static_cast<char>(std::toupper(ch));
    }
    return s;
}

std::string Formatter::formatPercentage(float percentage)
{
    float finalPercentage = percentage;
    if (percentage > 100.0) finalPercentage = 100.0;
    else if (percentage < 0.0) finalPercentage = 0.0;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << finalPercentage << " %";
    return oss.str();
}

std::string Formatter::formatPercentage(int percentage)
{
    int finalPercentage = percentage;
    if (percentage > 100) finalPercentage = 100;
    else if (percentage < 0) finalPercentage = 0;
    
    std::ostringstream oss;
    oss << finalPercentage << " %";
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

std::string Formatter::formatBPM(double bpm)
{
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(1) << bpm << " bpm";

    return oss.str();
}

std::string Formatter::formatFloat(const float value, const int precision)
{
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(precision) << value;

    return oss.str();
}


std::string Formatter::formatDouble(const double value, const int precision)
{
    std::ostringstream oss;

    oss << std::fixed << std::setprecision(precision) << value;

    return oss.str();
}

std::string Formatter::joinWithNewline(const std::vector<std::string>& lines)
{
    std::ostringstream oss;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        oss << lines.at(i);
        if (i != lines.size() - 1) {
            oss << '\n';
        }
    }
    return oss.str();
}

}
