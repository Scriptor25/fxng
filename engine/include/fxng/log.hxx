#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <string_view>

namespace fxng
{
    enum LogLevel
    {
        LogLevel_Verbose,
        LogLevel_Debug,
        LogLevel_Info,
        LogLevel_Warning,
        LogLevel_Error,
        LogLevel_Fatal,
    };

    template<typename... Arguments>
    void Log(const LogLevel log_level, std::string_view format, Arguments &&... arguments)
    {
        const auto timestamp = std::chrono::system_clock::now();
        const auto message = std::vformat(
            std::move(format),
            std::make_format_args(std::forward<Arguments>(arguments)...));
        std::cerr << '(' << timestamp << ')' << ' ' << '[' << log_level << ']' << ' ' << message << std::endl;
    }

    template<typename... Arguments>
    [[noreturn]] void Fatal(std::string_view format, Arguments &&... arguments)
    {
        Log(LogLevel_Fatal, std::forward<std::string_view>(format), std::forward<Arguments>(arguments)...);
        std::abort();
    }

    template<typename Condition, typename... Arguments>
    void Assert(Condition condition, std::string_view format, Arguments &&... arguments)
    {
        if (condition)
            return;

        Fatal(std::forward<std::string_view>(format), std::forward<Arguments>(arguments)...);
    }
}
