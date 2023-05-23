#include "log.h"
#include "raylib.h"
#include <argumentum/argparse-h.h>
#include <fmt/core.h>
#include <raylib-cpp.hpp>
#include <string_view>

constexpr int DEFAULT_SCREEN_WIDTH = 800;
constexpr int DEFAULT_SCREEN_HEIGHT = 600;
constexpr int DEFAULT_TARGET_FPS = 60;

void UpdateDrawFrame(raylib::Window&);

int main(int argc, char** argv) {
  auto args = argumentum::argument_parser();
  auto params = args.params();

  int screen_width;
  params.add_parameter(screen_width, "-w", "--width")
          .required(false)
          .default_value(DEFAULT_SCREEN_WIDTH)
          .maxargs(1);
  int screen_height;
  params.add_parameter(screen_height, "-h", "--height")
          .required(false)
          .default_value(DEFAULT_SCREEN_HEIGHT)
          .maxargs(1);
  int target_fps;
  params.add_parameter(target_fps, "-f", "--target-fps")
          .required(false)
          .default_value(DEFAULT_TARGET_FPS)
          .maxargs(1);
  args.add_default_help_option();
  args.parse_args(argc, argv);

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
