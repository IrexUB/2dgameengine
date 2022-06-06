#include <iostream>
#include <chrono>
#include <iomanip>

#include "../ANSI/ANSIColorModifier.h"
#include "Logger.h"

using namespace ansi_cm;

void Logger::Log(std::string_view msg)
{
    std::stringstream msgStream;

    auto currentDate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    msgStream << "LOG | " << std::put_time(localtime(&currentDate), "%F %T") << " - " << msg;

    std::cout << ColorModifier::Modify(msgStream.str(), Color::GREEN, FontStyle::BOLD) << std::endl;
}

void Logger::LogError(std::string_view msg)
{
    std::stringstream msgStream;

    auto currentDate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    msgStream << "ERR | " << std::put_time(localtime(&currentDate), "%F %T") << " - " << msg;

    std::cout << ColorModifier::Modify(msgStream.str(), Color::RED, FontStyle::BOLD) << std::endl;
}

void Logger::LogDebug(std::string_view msg)
{
    std::stringstream msgStream;

    auto currentDate = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    msgStream << "DBG | " << std::put_time(localtime(&currentDate), "%F %T") << " - " << msg;

    std::cout << ColorModifier::Modify(msgStream.str(), Color::YELLOW, FontStyle::BOLD) << std::endl;
}
