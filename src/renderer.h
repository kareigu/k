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

  struct State {
    struct CameraOffset {
      float x = 0.0f;
      float y = 0.0f;
    };
    CameraOffset camera_offset;
  };

  Renderer(const core::Config config) noexcept;
  ~Renderer() = default;

  ExitResult run_render_loop() noexcept;

private:
  void draw_debug_ui();

  std::unique_ptr<raylib::Window> m_window;
  core::Config m_config;
  State m_state;
};
}// namespace gfx
