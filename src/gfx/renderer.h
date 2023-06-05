#pragma once
#include "core/config.h"
#include <functional>
#include <memory>
#include <mutex>
#include <raylib-cpp.hpp>

namespace gfx {
class Renderer {
public:
  enum class ExitResult {
    Ok,
    UnknownError,
  };

  class State {
  public:
    struct CameraOffset {
      float x = 0.0f;
      float y = 0.0f;
    };

    CameraOffset camera_offset() const { return m_camera_offset; }
    void update_camera_offset(std::function<void(CameraOffset&)> update_fn);

  private:
    std::mutex m_mutex;
    CameraOffset m_camera_offset;
  };

  Renderer(const core::Config config) noexcept;
  ~Renderer() = default;

  ExitResult run_render_loop() noexcept;

  State& state() { return m_state; }

private:
  void draw_debug_ui();

  std::unique_ptr<raylib::Window> m_window;
  core::Config m_config;
  State m_state;
};
}// namespace gfx
