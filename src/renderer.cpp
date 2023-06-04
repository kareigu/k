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
    int render_width = m_window->GetRenderWidth();
    int render_height = m_window->GetRenderHeight();
    int tile_size = render_width / 32;

    auto mouse_pos = raylib::Vector2(GetMousePosition());

    m_window->BeginDrawing();
    ClearBackground(BLACK);

    for (int y = 0; y < render_height; y += tile_size) {
      for (int x = 0; x < render_width; x += tile_size) {
        auto rect = raylib::Rectangle(
                x + m_state.camera_offset().x,
                y + m_state.camera_offset().y,
                tile_size,
                tile_size);
        bool hovered = mouse_pos.CheckCollision(rect);
        rect.Draw(::Color(x, hovered ? 100 : 0, y, 255));
        if (m_config.debug())
          rect.DrawLines(::Color(255, 255, 255, 100));
      }
    }

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
  m_window->DrawFPS();
}

using State = Renderer::State;
void State::update_camera_offset(std::function<void(CameraOffset&)> update_fn) {
  m_mutex.lock();
  update_fn(m_camera_offset);
  m_mutex.unlock();
}
}// namespace gfx
