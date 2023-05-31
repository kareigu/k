#pragma once
#include "core/config.h"
#include <memory>
#include <raylib-cpp.hpp>

namespace gfx {
class Renderer {
public:
  enum class ExitResult {
    Ok,
    UnknownError,
  };

  Renderer(const core::Config config) noexcept;
  ~Renderer() = default;

  ExitResult run_render_loop() noexcept;

private:
  std::unique_ptr<raylib::Window> m_window;
};
}// namespace gfx
