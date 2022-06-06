#pragma once

#include <string>

enum class LogVerbosity
{
    MESSAGE,
    ERROR,
    FATAL_ERROR
};

class Logger
{
public:
    static void Log(std::string_view msg);
    static void LogError(std::string_view msg);
    static void LogDebug(std::string_view msg);
};