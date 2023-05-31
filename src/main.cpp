#include "core/assert.h"
#include "core/config.h"
#include "log.h"
#include "renderer.h"
#include <cstdlib>
#include <fmt/core.h>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string_view>


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

  gfx::Renderer renderer(config);
  auto render_loop_result = renderer.run_render_loop();

  switch (render_loop_result) {
    case gfx::Renderer::ExitResult::Ok:
      return EXIT_SUCCESS;
    case gfx::Renderer::ExitResult::UnknownError:
      log::error("Unknown error occurred");
      return EXIT_FAILURE;
  }
}
