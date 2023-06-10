#include "game_loop.h"
#include "core/assert.h"
#include "log.h"

namespace core {
constexpr float TARGET_PROCESS_RATE = 0.0083f;
constexpr long long TARGET_PROCESS_RATE_MILLIS = TARGET_PROCESS_RATE * 1000;

void GameLoop::start_game_loop_thread() {
  ASSERT(m_game_state != nullptr, "m_game_state not initialised");
  m_game_loop_thread = std::thread([this] {
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::duration<float> fsec;

    auto prev_time = Time::now();
    while (m_thread_should_run) {
      auto time_now = Time::now();
      float time_diff = fsec(time_now - prev_time).count();

      if (time_diff == 0.0f)
        continue;

      m_input_handler.process_inputs();
      bool input_handled = m_input_handler.handle_inputs(*m_game_state, time_diff);
      prev_time = time_now;

      if (time_diff < TARGET_PROCESS_RATE)
        std::this_thread::sleep_for(std::chrono::milliseconds(TARGET_PROCESS_RATE_MILLIS));
    }
  });
  log::info("GameLoop started");
}

void GameLoop::stop_game_loop_thread() {
  ASSERT(m_game_loop_thread.joinable(), "GameLoop thread is not joinable");
  m_thread_should_run = false;
  m_game_loop_thread.join();
  log::info("GameLoop stopped");
}
}// namespace core
