#pragma once

#include <fmt/core.h> // To be replaced by std::format in GCC 13 but I'm on GCC 12 here
#include <cstdio>
#include "magic_enum/magic_enum.hpp"

enum class LogLevel {
    Error,
    Warning,
    Info,
    Debug,
    Trace
};

constexpr LogLevel CompileTimeLogLevel = LogLevel::Trace;

template<LogLevel Level, typename... Args>
constexpr void log(fmt::format_string<Args...> fmtStr, Args&&... args)
{
    if constexpr (Level <= CompileTimeLogLevel)
    {
	    fmt::print("[{}]: ", magic_enum::enum_name<LogLevel>(CompileTimeLogLevel));
	    fmt::print(fmtStr, std::forward<Args>(args)...);
	    fmt::print("\n");
	    std::fflush(stdout);
    }
}

