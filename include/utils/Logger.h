#pragma once

#include "Singleton.h"

#include <string>
#include <vector>

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

    explicit Logger(std::string companyName);
    ~Logger() override = default;
    
    [[nodiscard]] std::vector<std::string> getLogs(int limit = 5, Level minLevel = DEBUG_LEVEL) const;

    void debug(const std::string& log, const std::string& source = "");
    void info(const std::string& log, const std::string& source = "");
    void warn(const std::string& log, const std::string& source = "");
    void error(const std::string& log, const std::string& source = "");
    
protected:
    std::string getDefaultAppName();
    std::string getAppName();

    [[nodiscard]] virtual int getMaxLatestLogFileLines() const { return _defaultMaxLatestLogFileLines; }
    [[nodiscard]] virtual int getMaxLogFileSize() const { return _defaultMaxLogFileSize; }
    [[nodiscard]] virtual long long getMaxLogFileLifetime() const { return _defaultMaxLogFileLifetime; }
    [[nodiscard]] virtual Level getDebugEntryLevel() const { return DEBUG_LEVEL; }
    [[nodiscard]] virtual Level getReleaseEntryLevel() const { return WARN_LEVEL; }

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
    juce::File _logDir {};
    juce::File currentLogFile {};
    juce::String currentDate {};

    [[nodiscard]] std::tm getCurrentTimestamp() const;
    [[nodiscard]] std::string getCurrentDateAndTime() const;
    [[nodiscard]] std::string getCurrentDate() const;
    [[nodiscard]] std::string levelToString(Level level) const;
    
    void addLog(Level level, const std::string& message, const std::string& source);

    void writeToFile(const std::string& logEntry, juce::File logFile);
    void writeToFile(const std::string& logEntry);
    void maintainLogFileSize(juce::File logFile);
    void cleanupOldLogs();
    
    juce::File getLatestLogFile();
    juce::File getTodaysLogFile(int fileNumber = 1);

    [[nodiscard]] std::string getLogAsString(Level level, const std::string& timetamp, const std::string& message, const std::string& source) const;
    [[nodiscard]] std::string getLogAsString(const Log& log) const;
};

class AppLogger
{
public:
    static void debug(const std::string& log, const std::string& source)
    {
        AppLogger::log(Logger::DEBUG_LEVEL, log, source);
    }

    static void info(const std::string& log, const std::string& source)
    {
        AppLogger::log(Logger::INFO_LEVEL, log, source);
    }

    static void warn(const std::string& log, const std::string& source)
    {
        AppLogger::log(Logger::WARN_LEVEL, log, source);
    }

    static void error(const std::string& log, const std::string& source)
    {
        AppLogger::log(Logger::ERROR_LEVEL, log, source);
    }

protected:
    static Logger& get();

private:
    static void log(const Logger::Level level, const std::string& log, const std::string& source)
    {
        return;
        if (!Logger::isAlive()) return;

        Logger& logger = get();
        switch (level)
        {
            case Logger::DEBUG_LEVEL:
                logger.debug(log, source);
                break;
            case Logger::INFO_LEVEL:
                logger.info(log, source);
                break;
            case Logger::WARN_LEVEL:
                logger.warn(log, source);
                break;
            case Logger::ERROR_LEVEL:
                logger.error(log, source);
                break;
        }
    }
};

}

