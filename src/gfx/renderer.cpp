#include "renderer.h"
#include "core/assert.h"
#include "gfx/tiles.h"
#include "log.h"
#include <fmt/core.h>
#include <raylib.h>


namespace gfx {

Renderer::Renderer(const core::Config config) noexcept {
  m_config = std::move(config);
  log::info("Creating window with {:d}x{:d}@{:d}", m_config.screen_width(), m_config.screen_height(), m_config.target_fps());
  m_window = std::make_unique<raylib::Window>(m_config.screen_width(), m_config.screen_height(), "kiikkupaska 2");
  m_window->SetTargetFPS(m_config.target_fps());
}

using ExitResult = Renderer::ExitResult;
ExitResult Renderer::run_render_loop() noexcept {
  ASSERT(m_window != nullptr, "No window");
  tiles::Map tilemap;
  while (!m_window->ShouldClose()) {
    int render_width = m_window->GetRenderWidth();
    int render_height = m_window->GetRenderHeight();
    int tile_size = render_width / 32;

    auto mouse_pos = raylib::Vector2(GetMousePosition());

    m_window->BeginDrawing();
    ClearBackground(BLACK);

    for (size_t y = 0; y < tiles::MAP_SIZE; y++) {
      for (size_t x = 0; x < tiles::MAP_SIZE; x++) {
        auto rect = raylib::Rectangle(
                x * tile_size + m_state.camera_offset().x,
                y * tile_size + m_state.camera_offset().y,
                tile_size,
                tile_size);
        bool hovered = mouse_pos.CheckCollision(rect);
        auto colour = tilemap.tiles()[x * y].colour();
        rect.Draw(hovered ? GREEN : colour);
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
  constexpr int X_POS = 10;
  constexpr unsigned int DEBUG_TEXT_COLOUR = 0xffff7eff;

  auto screen_width = m_window->GetRenderWidth();
  auto screen_height = m_window->GetRenderHeight();
  raylib::Text(
          fmt::format("MAP_SIZE = {:d}x{:d}", tiles::MAP_SIZE, tiles::MAP_SIZE),
          30,
          raylib::Color(DEBUG_TEXT_COLOUR),
          ::GetFontDefault(),
          2.0f)
          .Draw(X_POS, 60);
  raylib::Text(
          fmt::format("camera_offset = {{ .x = {:f}, .y = {:f} }}", m_state.camera_offset().x, m_state.camera_offset().y),
          30,
          raylib::Color(DEBUG_TEXT_COLOUR),
          ::GetFontDefault(),
          2.0f)
          .Draw(X_POS, 80);
  m_window->DrawFPS();
}

using State = Renderer::State;
void State::update_camera_offset(std::function<void(CameraOffset&)> update_fn) {
  m_mutex.lock();
  update_fn(m_camera_offset);
  m_mutex.unlock();
}
}// namespace gfx
