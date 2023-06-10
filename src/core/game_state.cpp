#include "game_state.h"

namespace core {
using PlayerPos = GameState::PlayerPos;
void PlayerPos::update_position(std::function<void(raylib::Vector2& pos)> fn) {
  m_mutex.lock();
  fn(m_pos);
  m_mutex.unlock();
}
}// namespace core
