#include "tiles.h"
#include <FastNoise/FastNoise.h>
#include <FastNoise/Generators/Fractal.h>
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
        colour = Color(181, 131, 77);
        break;
      case Type::Grass:
        colour = Color(101, 173, 37);
        break;
      case Type::Wood:
        colour = Color(96, 71, 12);
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

  static size_t calculate_index(long long x, long long y) {
    return (y + MAP_SIZE / 2) * MAP_SIZE + (x + MAP_SIZE / 2);
  };

  Map::Map() {
    auto seed = std::chrono::system_clock().now().time_since_epoch().count();
    SetRandomSeed(seed);
    auto simplex = FastNoise::New<FastNoise::Simplex>();
    auto fractal = FastNoise::New<FastNoise::FractalFBm>();
    fractal->SetSource(simplex);
    fractal->SetOctaveCount(5);

    std::array<float, MAP_SIZE * MAP_SIZE> noise_map;
    fractal->GenUniformGrid2D(noise_map.data(), 0, 0, MAP_SIZE, MAP_SIZE, 0.2f, seed);

    auto coordinates_to_tile_type = [&noise_map](long long x, long long y) -> Tile::Type {
      using Type = Tile::Type;

      float noise_value = noise_map[calculate_index(x, y)];


      if (noise_value > 0.4f)
        return Type::Wood;

      if (noise_value > 0.04f && noise_value < 0.08f)
        return Type::Stone;

      if (noise_value > 0.02f)
        return Type::Dirt;

      return Type::Grass;
    };


    for (long long y = -(MAP_SIZE / 2); y < static_cast<long long>(MAP_SIZE / 2); y++) {
      for (long long x = -(MAP_SIZE / 2); x < static_cast<long long>(MAP_SIZE / 2); x++) {
        Tile tile(raylib::Vector2(x, y));
        Tile::Type tile_type = coordinates_to_tile_type(x, y);
        bool is_wall = tile_type != Tile::Type::Dirt && tile_type != Tile::Type::Grass;
        tile.set_data(tile_type, is_wall ? Tile::Height::Wall : Tile::Height::Ground);
        size_t index = calculate_index(x, y);
        m_tilemap[index] = tile;
      }
    }
  }

  const Tile& Map::operator()(raylib::Vector2 pos) const {
    long long x = pos.x;
    long long y = pos.y;

    size_t index = calculate_index(x, y);
    return m_tilemap[index];
  }
}// namespace tiles
}// namespace gfx
