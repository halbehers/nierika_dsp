#pragma once

#include <string>

namespace nierika::gui {

class Formatter
{
    public:
    static std::string formatFrequency(float frequency);
    static std::string toUpper(const std::string& text);
    static std::string formatPercentage(float percentage);
    static std::string formatPercentage(int percentage);
    static std::string formatTimeInMs(float timeInMs);
    
    private:
    Formatter();
    ~Formatter();
};

}
