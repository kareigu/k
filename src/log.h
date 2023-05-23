#pragma once
#include "raylib.h"
#include <fmt/core.h>
#include <raylib-cpp.hpp>

namespace log {
template<typename... T>
void info(fmt::format_string<T...> string, T&&... args) {
  TraceLog(LOG_INFO, fmt::format(string, args...).c_str());
}

template<typename... T>
void error(fmt::format_string<T...> string, T&&... args) {
  TraceLog(LOG_ERROR, fmt::format(string, args...).c_str());
}

template<typename... T>
void warn(fmt::format_string<T...> string, T&&... args) {
  TraceLog(LOG_WARNING, fmt::format(string, args...).c_str());
}

template<typename... T>
void debug(fmt::format_string<T...> string, T&&... args) {
  TraceLog(LOG_DEBUG, fmt::format(string, args...).c_str());
}
}// namespace log
