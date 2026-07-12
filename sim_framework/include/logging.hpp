#pragma once

#include <print>
#include <cstdio>
#include "magic_enum/magic_enum.hpp"

enum class LogLevel {
    Error,
    Warning,
    Info,
    Debug,
    Trace
};

#ifdef VERBOSE
constexpr LogLevel CompileTimeLogLevel = LogLevel::Trace;
#else
constexpr LogLevel CompileTimeLogLevel = LogLevel::Debug;
#endif

template<LogLevel Level, typename... Args>
constexpr void log(std::format_string<Args...> fmtStr, Args&&... args) {
    if constexpr (Level <= CompileTimeLogLevel) {
        std::print("[{}]: ", magic_enum::enum_name<LogLevel>(CompileTimeLogLevel));
        std::println(fmtStr, std::forward<Args>(args)...);
        std::fflush(stdout);
    }
}

