#include "renderer.h"
#include "RenderTexture.hpp"
#include "Vector2.hpp"
#include "core/assert.h"
#include "gfx/tiles.h"
#include "log.h"
#include <fmt/core.h>
#include <optional>
#include <raylib.h>


namespace gfx {
constexpr unsigned int DEBUG_TEXT_COLOUR = 0xffff7eff;

Renderer::Renderer(const core::Config config, std::shared_ptr<core::GameState> game_state) noexcept : m_config(config), m_game_state(game_state) {
  log::info("Creating window with {:d}x{:d}@{:d}", m_config.screen_width(), m_config.screen_height(), m_config.target_fps());
  m_window = std::make_unique<raylib::Window>(m_config.screen_width(), m_config.screen_height(), "kiikkupaska 2");
  m_window->SetTargetFPS(m_config.target_fps());
}

using ExitResult = Renderer::ExitResult;
ExitResult Renderer::run_render_loop() noexcept {
  ASSERT(m_window != nullptr, "No window");

  constexpr uint8_t tile_size = 16;
  constexpr uint8_t tiles_on_width = 12;
  constexpr uint8_t tiles_on_height = 9;
  constexpr uint16_t render_width = tile_size * tiles_on_width;
  constexpr uint16_t render_height = tile_size * tiles_on_height;

  auto render_texture = raylib::RenderTexture2D(render_width, render_height);
  while (!m_window->ShouldClose()) {
    // int render_width = m_window->GetRenderWidth();
    // int render_height = m_window->GetRenderHeight();
    // int tile_size = render_width / 12;
    int screen_width = m_window->GetWidth();
    int screen_height = m_window->GetHeight();
    float render_scale = std::min(screen_width / render_width, screen_height / render_height);
    int screen_center_width = render_width / 2 - tile_size / 2;
    int screen_center_height = render_height / 2 - tile_size / 2;

    raylib::Vector2 mouse_pos = raylib::Vector2(GetMousePosition());
    raylib::Vector2 virtual_mouse_pos(
            (mouse_pos.x - (screen_width - render_width * render_scale) * 0.5f) / render_scale,
            (mouse_pos.y - (screen_height - render_height * render_scale) * 0.5f) / render_scale);
    virtual_mouse_pos.Clamp({0.0f, 0.0f}, {render_width, render_height});

    render_texture.BeginMode();
    ClearBackground(::Color(PINK));

    ASSERT(m_game_state != nullptr, "m_game_state was nullptr");
    const auto& player_pos = m_game_state->player_pos();
    const auto& tilemap = m_game_state->map();

    const tiles::Tile* debug_hovered_tile = nullptr;

    for (const auto& tile : tilemap()) {
      // if (tile.position().Distance(player_pos()) > tile_size / 2.0f)
      //   continue;

      float x_pos = screen_center_width + (-(tile.position().x - player_pos().x) * tile_size);
      float y_pos = screen_center_height + (-(tile.position().y - player_pos().y) * tile_size);
      if (x_pos < 0.0f || x_pos > render_width || y_pos < 0.0f || y_pos > render_height)
        continue;

      auto rect = raylib::Rectangle(
              x_pos,
              y_pos,
              tile_size,
              tile_size);
      bool hovered = virtual_mouse_pos.CheckCollision(rect);
      rect.Draw(hovered ? GREEN : tile.colour());
      if (m_config.debug()) {
        rect.DrawLines(::Color(255, 255, 255, 100));
        if (hovered)
          debug_hovered_tile = &tile;
      }
    }

    auto player_sprite_size = tile_size * 0.8f;
    auto player_sprite_diff_from_center = tile_size - player_sprite_size;
    auto player_sprite = raylib::Rectangle(
            screen_center_width + player_sprite_diff_from_center,
            screen_center_height + player_sprite_diff_from_center,
            player_sprite_size,
            player_sprite_size);
    player_sprite.DrawRounded(10.0f, 8, ::Color(255, 0, 120, 255));
    render_texture.EndMode();


    m_window->BeginDrawing();
    m_window->ClearBackground();
    DrawTexturePro(
            render_texture.GetTexture(),
            {0.0f, 0.0f, render_width, -render_height},
            {(screen_width - (render_width * render_scale)) * 0.5f,
             (screen_height - (render_height * render_scale)) * 0.5f,
             render_width * render_scale,
             render_height * render_scale},
            {0, 0},
            0.0f,
            WHITE);
    if (m_config.debug()) {
      draw_debug_ui();
      if (debug_hovered_tile) {
        raylib::Text(
                fmt::format(
                        "x = {:f}\ny = {:f}\ntype = {:d}\nheight = {:d}",
                        debug_hovered_tile->position().x,
                        debug_hovered_tile->position().y,
                        static_cast<size_t>(debug_hovered_tile->type()),
                        static_cast<size_t>(debug_hovered_tile->height())),
                10.0f,
                raylib::Color(DEBUG_TEXT_COLOUR))
                .Draw(::Vector2(mouse_pos.x + 20.0f, mouse_pos.y));
      }
    }

    m_window->EndDrawing();
  }

  return ExitResult::Ok;
}


void Renderer::draw_debug_ui() {
  constexpr int X_POS = 10;

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
          .Draw(X_POS, 90);
  ASSERT(m_game_state != nullptr, "m_game_state was nullptr");
  const auto& player_pos = m_game_state->player_pos();
  raylib::Text(
          fmt::format("player_position = {{ .x = {:f}, .y = {:f} }}", player_pos().x, player_pos().y),
          30,
          raylib::Color(DEBUG_TEXT_COLOUR),
          ::GetFontDefault(),
          2.0f)
          .Draw(X_POS, 120);
  m_window->DrawFPS();
}

using State = Renderer::State;
void State::update_camera_offset(std::function<void(CameraOffset&)> update_fn) {
  m_mutex.lock();
  update_fn(m_camera_offset);
  m_mutex.unlock();
}
}// namespace gfx
