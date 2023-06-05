#include "tiles.h"
#include <chrono>
#include <raylib.h>

namespace gfx {
namespace tiles {
  Tile::Tile() {
    update_colour();
  }
  Tile::Tile(Type type, Position position) : m_type(type), m_position(position) {
    update_colour();
  }
  Tile::Tile(Type type) : m_type(type) {
    update_colour();
  }
  Tile::Tile(Position position) : m_position(position) {
    update_colour();
  }

  void Tile::set_type(Type type) {
    if (type == m_type)
      return;

    m_type = type;
    update_colour();
  }

  void Tile::set_position(Position position) {
    if (position == m_position)
      return;

    m_position = position;
    update_colour();
  }

  void Tile::set_data(Type type, Position position) {
    if (type == m_type && position == m_position)
      return;

    m_type = type;
    m_position = position;
    update_colour();
  }

  void Tile::update_colour() {
    using raylib::Color;
    Color colour;
    switch (m_type) {
      case Type::Stone:
        colour = Color(10, 10, 10);
        break;
      case Type::Dirt:
        colour = Color(96, 71, 12);
        break;
      case Type::Grass:
        colour = Color(101, 173, 37);
        break;
      case Type::Wood:
        colour = Color(181, 131, 77);
        break;
      default:
        colour = Color(255, 0, 252);
        break;
    }

    switch (m_position) {
      case Position::Ground:
        break;
      case Position::Wall:
        colour.AlphaBlend(::Color(20, 20, 20), ::Color(20, 20, 20));
        break;
      case Position::Ceiling:
        colour.AlphaBlend(::Color(50, 50, 50), ::Color(50, 50, 50));
        break;
    }

    m_colour = std::move(colour);
  }

  Map::Map() {
    SetRandomSeed(std::chrono::system_clock().now().time_since_epoch().count());
    for (auto& tile : m_tilemap) {
      tile.set_data(static_cast<Tile::Type>(GetRandomValue(0, 3)), static_cast<Tile::Position>(GetRandomValue(0, 2)));
    }
  }
}// namespace tiles
}// namespace gfx
