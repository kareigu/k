#include "assert.h"
#include <cstdlib>
#include <fmt/color.h>
#include <fmt/core.h>

namespace core {
void __ASSERT(const char* expr_str, bool expr, const char* file, int line, const char* msg) {
  if (!expr) {
    fmt::print(
            stderr,
            fmt::emphasis::bold | fg(fmt::color::orange_red),
            "ASSERT: {:s}\n",
            fmt::styled(msg, fg(fmt::color::white)));
    fmt::print(
            stderr,
            fmt::emphasis::bold | fg(fmt::color::light_green),
            "EXPECTED: {:s}\n",
            fmt::styled(expr_str, fg(fmt::color::white)));
    fmt::print(
            stderr,
            fmt::emphasis::bold | fg(fmt::color::orange),
            "IN: {:s}:{:d}\n",
            fmt::styled(file, fmt::emphasis::underline | fg(fmt::color::light_blue)),
            fmt::styled(line, fg(fmt::color::orange)));
    abort();
  }
}
}// namespace core
