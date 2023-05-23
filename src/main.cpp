#include "log.h"
#include "raylib.h"
#include <argumentum/argparse-h.h>
#include <fmt/core.h>
#include <raylib-cpp.hpp>
#include <string_view>

constexpr int screen_width = 800;
constexpr int screen_height = 600;
constexpr int target_fps = 60;

void UpdateDrawFrame(raylib::Window&);

int main() {
  log::info("Creating window with {:d}x{:d}@{:d}", screen_width, screen_height, target_fps);
  raylib::Window window(screen_width, screen_height, "Raygame");

  SetTargetFPS(target_fps);
  while (!window.ShouldClose()) {
    UpdateDrawFrame(window);
  }

  return 0;
}

void UpdateDrawFrame(raylib::Window& window) {
  window.BeginDrawing();
  ClearBackground(BLACK);
  raylib::Text(fmt::format("{:d} fps | {:f} ms", window.GetFPS(), window.GetFrameTime()), 30, RAYWHITE, ::GetFontDefault(), 2.0f).Draw(190, 200);
  window.EndDrawing();
}
