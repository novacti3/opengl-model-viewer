#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

enum LogLevel
{
    Info = 0, Warning, Error, Fatal
};

// TODO: Logging into file per channel regardless of current filter
// TODO: Color highlighting/output
// TODO: Channels (eg. a channel for general stuff, another channel for rendering)
// TODO: Channel filters (eg. only print general and rendering stuff) (through a bitmask again probably)
// NOTE: Crash dump?
class Log final
{
    private:
    // NOTE: A bit mask might be better
    inline static LogLevel _levelFilter = LogLevel::Info;

    private:
    Log() {}
    ~Log() {}

    public:
    static void SetLogLevelFilter(LogLevel filter)
    {
        Log::_levelFilter = filter;
    }

    static void LogInfo(const char *message)            { Log::LogMessage(LogLevel::Info, message); }
    static void LogInfo(const std::string &message)     { Log::LogMessage(LogLevel::Info, message); }

    static void LogWarning(const char *message)         { Log::LogMessage(LogLevel::Warning, message); }
    static void LogWarning(const std::string &message)  { Log::LogMessage(LogLevel::Warning, message); }

    static void LogError(const char *message)           { Log::LogMessage(LogLevel::Error, message); }
    static void LogError(const std::string &message)    { Log::LogMessage(LogLevel::Error, message); }

    static void LogFatal(const char *message)           { Log::LogMessage(LogLevel::Fatal, message); }
    static void LogFatal(const std::string &message)    { Log::LogMessage(LogLevel::Fatal, message); }

    private:
    // TODO: String formatting
    static void LogMessage(LogLevel severity, const char *message)
    {
        LogMessage(severity, std::string(message));
    }
    static void LogMessage(LogLevel severity, const std::string &message)
    {
        if(severity < (int)Log::_levelFilter)
        {
            return;
        }

        // Get the current time in HH:MM:SS format
        time_t rawCurrentTime;
        time(&rawCurrentTime);
        tm *currentTime = localtime(&rawCurrentTime);
        
        char currentTimeStr[9];
        strftime(currentTimeStr, 9, "%T", currentTime);

        // Get the log level
        // NOTE: There is probably a more elegant and better way of converting the enum to a string but I can't be arsed right now
        std::string logLevelStr;
        switch(severity)
        {
            case LogLevel::Info:
            {
                logLevelStr = "Info";
            }
            break;
        
            case LogLevel::Warning:
            {
                logLevelStr = "Warning";
            }
            break;

            case LogLevel::Error:
            {
                logLevelStr = "Error";
            }
            break;

            case LogLevel::Fatal:
            {
                logLevelStr = "Fatal";
            }
            break;
        }

        std::cout << "[" << currentTimeStr << "] " << logLevelStr << ": " << message << std::endl;

        // TODO: Log message event so the message gets printed out to the Console UI window as well
    }
};

static bool CheckError(char const* file, char const* function, int line)
{
    if (auto error = glGetError(); error != GL_NO_ERROR)
    {
        Log::LogError("OpenGL Error: " + std::to_string(error) + " : " + file + ":" + function + ":" + std::to_string(line));
        return false;
    }
    return true;
}

#define GL_CALL(x) x; CheckError(__FILE__, #x, __LINE__);