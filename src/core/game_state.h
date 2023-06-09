#pragma once
#include "gfx/tiles.h"
#include <functional>
#include <mutex>
#include <raylib-cpp.hpp>


namespace core {
class GameState {
public:
  class PlayerPos {
  public:
    PlayerPos() = default;
    ~PlayerPos() = default;
    const raylib::Vector2& operator()() const { return m_pos; }

    void update_position(const gfx::tiles::Map& map, std::function<void(const gfx::tiles::Map& map, raylib::Vector2& pos)>);

  private:
    std::mutex m_mutex;
    raylib::Vector2 m_pos;
  };

  PlayerPos& player_pos() { return m_player_pos; }
  const gfx::tiles::Map& map() const { return m_map; }

  inline void update_player_position(std::function<void(const gfx::tiles::Map& map, raylib::Vector2& pos)> fn) { m_player_pos.update_position(m_map, fn); };

  GameState() = default;
  GameState(const GameState&) = delete;
  GameState(GameState&&) = delete;
  GameState& operator=(const GameState&) = delete;
  GameState& operator=(GameState&&) = delete;
  ~GameState() = default;

private:
  PlayerPos m_player_pos;
  gfx::tiles::Map m_map;
};

}// namespace core
