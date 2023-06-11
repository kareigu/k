#include "tiles.h"
#include <chrono>
#include <raylib.h>

namespace gfx {
namespace tiles {
  Tile::Tile(raylib::Vector2 position) : m_position(position) {
    update_colour();
  }
  Tile::Tile(raylib::Vector2 position, Type type, Height height)
      : m_type(type), m_height(height), m_position(position) {
    update_colour();
  }
  Tile::Tile(raylib::Vector2 position, Type type) : m_type(type), m_position(position) {
    update_colour();
  }
  Tile::Tile(raylib::Vector2 position, Height height) : m_height(height), m_position(position) {
    update_colour();
  }

  void Tile::set_type(Type type) {
    if (type == m_type)
      return;

    m_type = type;
    update_colour();
  }

  void Tile::set_height(Height height) {
    if (height == m_height)
      return;

    m_height = height;
    update_colour();
  }

  void Tile::set_data(Type type, Height height) {
    if (type == m_type && height == m_height)
      return;

    m_type = type;
    m_height = height;
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

    switch (m_height) {
      case Height::Ground:
        break;
      case Height::Wall:
        colour.AlphaBlend(::Color(20, 20, 20), ::Color(20, 20, 20));
        break;
      case Height::Ceiling:
        colour.AlphaBlend(::Color(50, 50, 50), ::Color(50, 50, 50));
        break;
    }

    m_colour = std::move(colour);
  }

  Map::Map() {
    SetRandomSeed(std::chrono::system_clock().now().time_since_epoch().count());
    for (long long y = -(MAP_SIZE / 2); y < static_cast<long long>(MAP_SIZE / 2); y++) {
      for (long long x = -(MAP_SIZE / 2); x < static_cast<long long>(MAP_SIZE / 2); x++) {
        Tile tile(raylib::Vector2(x, y));
        tile.set_data(static_cast<Tile::Type>(GetRandomValue(0, 3)), static_cast<Tile::Height>(GetRandomValue(0, 2)));
        size_t index = (y + MAP_SIZE / 2) * MAP_SIZE + (x + MAP_SIZE / 2);
        m_tilemap[index] = tile;
      }
    }
  }
}// namespace tiles
}// namespace gfx
