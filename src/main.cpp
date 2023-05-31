#include "core/assert.h"
#include "core/config.h"
#include "log.h"
#include <cstdlib>
#include <fmt/core.h>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string_view>


void UpdateDrawFrame(raylib::Window&);

int main(int argc, char** argv) {
  auto config_result = core::Config::create_from_args(argc, argv);
  if (!config_result) {
    using Error = core::Config::Error;
    switch (config_result.error()) {
      case Error::HelpShown:
        log::debug("Help screen shown {:d}", 5);
        break;
      case Error::InvalidArguments:
        log::error("Invalid arguments given");
        break;
      case Error::Unknown:
        log::error("Unknown error occurred");
        break;
    }
    return EXIT_FAILURE;
  }

  auto config = config_result.value();

  log::info("Creating window with {:d}x{:d}@{:d}", config.screen_width(), config.screen_height(), config.target_fps());
  raylib::Window window(config.screen_width(), config.screen_height(), "Raygame");

  SetTargetFPS(config.target_fps());
  while (!window.ShouldClose()) {
    UpdateDrawFrame(window);
  }

  return EXIT_SUCCESS;
}

void UpdateDrawFrame(raylib::Window& window) {
  window.BeginDrawing();
  ClearBackground(BLACK);
  raylib::Text(fmt::format("{:d} fps | {:f} ms", window.GetFPS(), window.GetFrameTime()), 30, RAYWHITE, ::GetFontDefault(), 2.0f).Draw(190, 200);
  window.EndDrawing();
}
