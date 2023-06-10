#pragma once
#include "core/game_state.h"
#include "core/input.h"
#include <memory>


namespace core {
class GameLoop {
public:
  GameLoop(std::shared_ptr<GameState> game_state) : m_game_state(game_state){};
  ~GameLoop() = default;


  void start_game_loop_thread();
  void stop_game_loop_thread();

private:
  std::shared_ptr<GameState> m_game_state;

  input::Handler m_input_handler;

  std::atomic<bool> m_thread_should_run = true;
  std::thread m_game_loop_thread;
};
}// namespace core
