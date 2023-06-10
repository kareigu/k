#include "core/assert.h"
#include "core/config.h"
#include "core/game_loop.h"
#include "core/game_state.h"
#include "core/input.h"
#include "gfx/renderer.h"
#include "log.h"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fmt/core.h>
#include <memory>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string_view>
#include <thread>


int main(int argc, char** argv) {
  log::info("Reading configuration");
  auto config_result = core::Config::create_from_args(argc, argv);
  if (!config_result) {
    using Error = core::Config::Error;
    log::info("Test {:d}", (int) config_result.error());
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
      default:
        log::error("Unhandled error case while reading configuration: {:d}", (int) config_result.error());
        break;
    }
    log::error("Exiting");
    return EXIT_FAILURE;
  }
  log::info("Configuration read successfully");

  auto config = config_result.value();

  auto game_state = std::make_shared<core::GameState>();
  core::GameLoop game_loop(game_state);

  gfx::Renderer renderer(config, game_state);

  game_loop.start_game_loop_thread();
  auto render_loop_result = renderer.run_render_loop();

  game_loop.stop_game_loop_thread();
  switch (render_loop_result) {
    case gfx::Renderer::ExitResult::Ok:
      return EXIT_SUCCESS;
    case gfx::Renderer::ExitResult::UnknownError:
      log::error("Unknown error occurred");
      return EXIT_FAILURE;
  }
}
