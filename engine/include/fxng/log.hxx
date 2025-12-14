#pragma once

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <string_view>
#include <unordered_map>

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

    inline std::ostream &operator<<(std::ostream &stream, const LogLevel log_level)
    {
        static const std::unordered_map<LogLevel, const char *> map
        {
            { LogLevel_Verbose, "Verbose" },
            { LogLevel_Debug, "Debug" },
            { LogLevel_Info, "Info" },
            { LogLevel_Warning, "Warning" },
            { LogLevel_Error, "Error" },
            { LogLevel_Fatal, "Fatal" },
        };
        return stream << map.at(log_level);
    }

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

template<>
struct std::formatter<std::filesystem::path> : std::formatter<std::string>
{
    template<typename Context>
    auto format(const std::filesystem::path &path, Context &context) const
    {
        return std::formatter<std::string>::format(path.string(), context);
    }
};

template<typename T>
struct std::formatter<std::optional<T>> : std::formatter<T>
{
    template<typename Context>
    auto format(const std::optional<T> &opt, Context &context) const
    {
        if (opt.has_value())
            return std::formatter<T>::format(opt.value(), context);
        return std::format_to(context.out(), "(null)");
    }
};
