#include "renderer.h"
#include "core/assert.h"
#include "log.h"
#include <fmt/core.h>


namespace gfx {
constexpr unsigned int DEBUG_TEXT_COLOUR = 0x00ff7eff;

Renderer::Renderer(const core::Config config) noexcept {
  m_config = std::move(config);
  log::info("Creating window with {:d}x{:d}@{:d}", m_config.screen_width(), m_config.screen_height(), m_config.target_fps());
  m_window = std::make_unique<raylib::Window>(m_config.screen_width(), m_config.screen_height(), "Raygame");
  m_window->SetTargetFPS(m_config.target_fps());
}

using ExitResult = Renderer::ExitResult;
ExitResult Renderer::run_render_loop() noexcept {
  ASSERT(m_window != nullptr, "No window");
  while (!m_window->ShouldClose()) {
    m_window->BeginDrawing();
    ClearBackground(BLACK);

    if (m_config.debug())
      draw_debug_ui();

    m_window->EndDrawing();
  }

  return ExitResult::Ok;
}


void Renderer::draw_debug_ui() {
  raylib::Text(
          fmt::format("{:d} fps | {:f} ms", m_window->GetFPS(), m_window->GetFrameTime()),
          30,
          raylib::Color(DEBUG_TEXT_COLOUR),
          ::GetFontDefault(),
          2.0f)
          .Draw(190, 200);
}
}// namespace gfx
