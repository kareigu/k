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
  void draw_debug_ui();

  std::unique_ptr<raylib::Window> m_window;
  core::Config m_config;
};
}// namespace gfx
