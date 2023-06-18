#include "game_state.h"

namespace core {
using PlayerPos = GameState::PlayerPos;
void PlayerPos::update_position(const gfx::tiles::Map& map, std::function<void(const gfx::tiles::Map& map, raylib::Vector2& pos)> fn) {
  m_mutex.lock();
  fn(map, m_pos);
  m_mutex.unlock();
}
}// namespace core
