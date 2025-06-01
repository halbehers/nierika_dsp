 #pragma once

#include <utility>

#include "../../include/utils/Logger.h"

namespace nierika::utils
{

    Logger::Logger(std::string companyName):
        _companyName(std::move(companyName))
    {
        _logDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                        .getChildFile(_companyName).getChildFile(getAppName()).getChildFile("logs");

        (void) _logDir.createDirectory();
        cleanupOldLogs();
    }

    std::vector<std::string> Logger::getLogs(const int limit, const Level minLevel) const
    {
        std::vector<std::string> result;
        int count = 0;
        const int logsSize = static_cast<int>(_logs.size());

        for (int i = logsSize - 1; i >= 0 && count < limit; --i)
            {
                if (const auto log = _logs[static_cast<std::size_t>(i)]; log.level >= minLevel)
                {
                    result.push_back(getLogAsString(log));
                    ++count;
                }
            }

        return result;
    }

    void Logger::debug(const std::string& log, const std::string& source)
    {
        addLog(DEBUG_LEVEL, log, source);
    }

    void Logger::info(const std::string& log, const std::string& source)
    {
        addLog(INFO_LEVEL, log, source);
    }

    void Logger::warn(const std::string& log, const std::string& source)
    {
        addLog(WARN_LEVEL, log, source);
    }

    void Logger::error(const std::string& log, const std::string& source)
    {
        addLog(ERROR_LEVEL, log, source);
    }

    std::string Logger::getDefaultAppName()
    {
        return "Default";
    }

    std::string Logger::getAppName()
    {
        if (auto* app = juce::JUCEApplication::getInstance())
            return app->getApplicationName().toStdString();
    
        auto pluginName = juce::PluginDescription().name.toStdString();
        
        if (pluginName != "") return pluginName;
    
        return getDefaultAppName();
    }

    std::tm Logger::getCurrentTimestamp() const
    {
        const auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        std::tm tm {};
    #if defined(_WIN32)
        localtime_s(&tm, &timeT);
    #else
        localtime_r(&timeT, &tm);
    #endif
        return tm;
    }

    std::string Logger::getCurrentDateAndTime() const
    {
        std::ostringstream oss;
        const std::tm tm = getCurrentTimestamp();
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string Logger::getCurrentDate() const
    {
        const std::tm tm = getCurrentTimestamp();
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    }

    std::string Logger::levelToString(Level level) const
    {
        switch (level)
        {
            case DEBUG_LEVEL: return "DEBUG";
            case INFO_LEVEL: return "INFO";
            case WARN_LEVEL: return "WARN";
            case ERROR_LEVEL: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    void Logger::addLog(Level level, const std::string& message, const std::string& source)
    {
        cleanupOldLogs();

        const auto time = getCurrentDateAndTime();
        _logs.push_back({ message, level, time, source });

    #ifndef NDEBUG
        if (level >= getDebugEntryLevel())
            writeToFile(getLogAsString(level, time, message, source));
    #else
        if (level >= getReleaseEntryLevel())
            writeToFile(getLogAsString(level, time, message, source));
    #endif
    }

    void Logger::writeToFile(const std::string& logEntry, juce::File logFile)
    {
        if (juce::FileOutputStream stream(logFile, 512); stream.openedOk())
        {
            stream.setPosition(logFile.getSize());
            stream.writeText(juce::String(logEntry) + "\n", false, false, nullptr);
            stream.flush();
        }
    }

    void Logger::writeToFile(const std::string& logEntry)
    {
        const auto latestLogFile = getLatestLogFile();

        writeToFile(logEntry, latestLogFile);

        maintainLogFileSize(latestLogFile);

        auto todaysLogFile = getTodaysLogFile();
        writeToFile(logEntry, todaysLogFile);
    }

    void Logger::maintainLogFileSize(juce::File logFile)
    {
        juce::StringArray lines;
        lines.addLines(logFile.loadFileAsString());

        if (lines.size() > getMaxLatestLogFileLines())
        {
            juce::StringArray trimmed;
            trimmed.addArray(lines, lines.size() - getMaxLatestLogFileLines(), getMaxLatestLogFileLines());

            (void) logFile.replaceWithText(trimmed.joinIntoString("\n"));
        }
    }

    void Logger::cleanupOldLogs()
    {
        auto files = _logDir.findChildFiles(juce::File::findFiles, false, "*.log");

        const auto now = juce::Time::getCurrentTime();

        for (auto& file : files)
        {
            if (file.getFileName() == "latest.log") continue;

            if (auto modified = file.getLastModificationTime(); now.toMilliseconds() - modified.toMilliseconds() > getMaxLogFileLifetime())
            {
                (void) file.deleteFile();
            }
        }
    }

    juce::File Logger::getLatestLogFile()
    {
        auto file = _logDir.getChildFile("latest.log");
        if (!file.existsAsFile()) (void) file.create();
        return file;
    }

    juce::File Logger::getTodaysLogFile(const int fileNumber)
    {
        currentDate = getCurrentDate();
        std::string appName = getAppName();
        std::transform(appName.begin(), appName.end(), appName.begin(),
            [](unsigned char c){ return std::tolower(c); });
        std::ostringstream oss;
        oss << appName << "_" << currentDate << (fileNumber > 1 ? "_" + std::to_string(fileNumber) : "") << ".log";

        auto file = _logDir.getChildFile(oss.str());
        if (!file.existsAsFile())
        {
            (void) file.create();
            return file;
        }
        if (file.getSize() > getMaxLogFileSize())
        {
            return getTodaysLogFile(fileNumber + 1);
        }
        return file;
    }

    std::string Logger::getLogAsString(Level level, const std::string& timetamp, const std::string& message, const std::string& source) const
    {
        std::ostringstream oss;
        oss << "[" << timetamp << "] "
            << "[" << levelToString(level) << "] "
            << (!source.empty() ? "[" + source + "] " : "")
            << message;
        return oss.str();
    }

    std::string Logger::getLogAsString(const Log& log) const
    {
        return getLogAsString(log.level, log.timestamp, log.message, log.source);
    }

}
