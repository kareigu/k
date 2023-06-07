#include "core/assert.h"
#include "core/config.h"
#include "core/input.h"
#include "gfx/renderer.h"
#include "log.h"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fmt/core.h>
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

  gfx::Renderer renderer(config);

  std::atomic_bool stop_input_thread = false;
  std::thread input_thread([&stop_input_thread, &renderer] {
    core::input::Handler input_handler;

    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::duration<float> fsec;

    auto prev_time = Time::now();
    while (!stop_input_thread) {
      auto time_now = Time::now();
      fsec time_diff = time_now - prev_time;

      if (time_diff.count() == 0.0f)
        continue;

      input_handler.process_inputs();
      bool input_handled = input_handler.handle_inputs(renderer.state(), time_diff.count());
      prev_time = time_now;

      if (!input_handled)
        std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
  });
  auto render_loop_result = renderer.run_render_loop();

  stop_input_thread = true;
  input_thread.join();

  switch (render_loop_result) {
    case gfx::Renderer::ExitResult::Ok:
      return EXIT_SUCCESS;
    case gfx::Renderer::ExitResult::UnknownError:
      log::error("Unknown error occurred");
      return EXIT_FAILURE;
  }
}
