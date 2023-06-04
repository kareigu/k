#include "core/assert.h"
#include "core/config.h"
#include "log.h"
#include "renderer.h"
#include <atomic>
#include <cstdlib>
#include <fmt/core.h>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <string_view>
#include <thread>


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

  std::atomic_bool stop_input_thread = false;
  std::jthread input_thread([&stop_input_thread, &renderer] {
    auto prev_time = std::chrono::system_clock::now().time_since_epoch().count();
    while (!stop_input_thread) {
      auto time_now = std::chrono::system_clock::now().time_since_epoch().count();
      auto time_diff = time_now - prev_time;

      if (time_diff == 0)
        continue;

      gfx::Renderer::State::CameraOffset new_offset;
      if (IsKeyDown(KEY_DOWN))
        new_offset.y -= 0.4f / time_diff;
      if (IsKeyDown(KEY_UP))
        new_offset.y += 0.4f / time_diff;
      if (IsKeyDown(KEY_LEFT))
        new_offset.x += 0.4f / time_diff;
      if (IsKeyDown(KEY_RIGHT))
        new_offset.x -= 0.4f / time_diff;

      if (new_offset.x != 0.0f || new_offset.y != 0.0f)
        log::info("new_offset {{ .x = {:f}, .y = {:f} }}", new_offset.x, new_offset.y);

      renderer.state().update_camera_offset([new_offset](auto& offset) {
        offset.x += new_offset.x;
        offset.y += new_offset.y;
      });

      prev_time = time_now;
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
