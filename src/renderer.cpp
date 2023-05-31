#include "renderer.h"
#include "core/assert.h"
#include "log.h"
#include <fmt/core.h>


namespace gfx {

Renderer::Renderer(const core::Config config) noexcept {
  log::info("Creating window with {:d}x{:d}@{:d}", config.screen_width(), config.screen_height(), config.target_fps());
  m_window = std::make_unique<raylib::Window>(config.screen_width(), config.screen_height(), "Raygame");
  m_window->SetTargetFPS(config.target_fps());
}

using ExitResult = Renderer::ExitResult;
ExitResult Renderer::run_render_loop() noexcept {
  ASSERT(m_window != nullptr, "No window");
  while (!m_window->ShouldClose()) {
    m_window->BeginDrawing();
    ClearBackground(BLACK);
    raylib::Text(fmt::format("{:d} fps | {:f} ms", m_window->GetFPS(), m_window->GetFrameTime()), 30, RAYWHITE, ::GetFontDefault(), 2.0f).Draw(190, 200);
    m_window->EndDrawing();
  }

  return ExitResult::Ok;
}
}// namespace gfx
