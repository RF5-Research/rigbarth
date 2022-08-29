#pragma once
#include <fmt/format.h>
#include <fmt/color.h>
#include "Constants.hpp"

enum LogType
{
    Info,
    Warning,
    Error
};

void vlog(
    LogType type,
    fmt::string_view format,
    fmt::format_args args
);

template <typename S, typename... Args>
void log(
    LogType type,
    const S& format,
    Args&&... args
) {
    vlog(type, format, fmt::make_format_args(args...));
}

#define LOG_INFO(format, ...) \
    log(LogType::Info, FMT_STRING(format), __VA_ARGS__)
#define LOG_WARNING(format, ...) \
    log(LogType::Warning, FMT_STRING(format), __VA_ARGS__)
#define LOG_ERROR(format, ...) \
    log(LogType::Error, FMT_STRING(format), __VA_ARGS__)