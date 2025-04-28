#pragma once

#include "Singleton.h"

#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace nierika::utils
{

class Logger: public Singleton<Logger>
{
public:
    enum Level
    {
        DEBUG_LEVEL,
        INFO_LEVEL,
        WARN_LEVEL,
        ERROR_LEVEL
    };

    Logger(const std::string& companyName);
    ~Logger();
    
    std::vector<std::string> getLogs(const int limit = 5, const Level minLevel = DEBUG_LEVEL) const;

    void debug(const std::string& log, const std::string& source = "");
    void info(const std::string& log, const std::string& source = "");
    void warn(const std::string& log, const std::string& source = "");
    void error(const std::string& log, const std::string& source = "");
    
protected:
    std::string getDefaultAppName();
    std::string getAppName();

    virtual const int getMaxLatestLogFileLines() const { return _defaultMaxLatestLogFileLines; }
    virtual const int getMaxLogFileSize() const { return _defaultMaxLogFileSize; }
    virtual const long long getMaxLogFileLifetime() const { return _defaultMaxLogFileLifetime; }
    virtual const Level getDebugEntryLevel() const { return DEBUG_LEVEL; }
    virtual const Level getReleaseEntryLevel() const { return WARN_LEVEL; }

private:
    struct Log
    {
        std::string message;
        Level level;
        std::string timestamp;
        std::string source;
        
    };

    static constexpr int _defaultMaxLatestLogFileLines = 10000;
    static constexpr int _defaultMaxLogFileSize = 10 * 1024 * 1024; // 10 MB - Does not apply to "latest.log"
    static constexpr long long _defaultMaxLogFileLifetime = 30LL * 24 * 60 * 60 * 1000; // 30 days

    std::vector<Log> _logs;

    std::string _companyName;
    juce::File _logDir;
    juce::File currentLogFile;
    juce::String currentDate;
    int fileIndex = 0;

    void setup();

    std::tm getCurrentTimestamp() const;
    std::string getCurrentDateAndTime() const;
    std::string getCurrentDate() const;
    std::string levelToString(Level level) const;
    
    void addLog(Level level, const std::string& message, const std::string& source);

    void writeToFile(const std::string& logEntry, juce::File logFile);
    void writeToFile(const std::string& logEntry);
    void maintainLogFileSize(juce::File logFile);
    void cleanupOldLogs();
    
    juce::File getLatestLogFile();
    juce::File getTodaysLogFile(const int fileNumber = 1);

    std::string getLogAsString(Level level, const std::string& timetamp, const std::string& message, const std::string& source) const;
    std::string getLogAsString(const Log& log) const;
};

struct AppLogger
{
    static Logger& get();
};

}

