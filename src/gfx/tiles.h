#pragma once
#include <Color.hpp>
#include <Vector2.hpp>
#include <array>


namespace gfx {
namespace tiles {
  constexpr size_t MAP_SIZE = 64;

  class Tile {
  public:
    enum class Type {
      Grass,
      Dirt,
      Stone,
      Wood,
    };

    enum class Height {
      Ground,
      Wall,
      Ceiling,
    };

    Tile() = default;
    Tile(raylib::Vector2);
    Tile(raylib::Vector2, Type, Height);
    Tile(raylib::Vector2, Type);
    Tile(raylib::Vector2, Height);

    ~Tile() = default;

    Type type() const { return m_type; }
    Height height() const { return m_height; }
    raylib::Vector2 position() const { return m_position; }

    void set_type(Type);
    void set_height(Height);
    void set_data(Type, Height);


    [[nodiscard]] raylib::Color colour() const { return m_colour; };

  private:
    void update_colour();

    Type m_type = Type::Stone;
    Height m_height = Height::Ground;
    raylib::Color m_colour = raylib::Color(10, 10, 10);
    raylib::Vector2 m_position;
  };

  class Map {
  public:
    Map();
    ~Map() = default;

    const auto& tiles() const { return m_tilemap; }

    const auto& operator()() const { return m_tilemap; }

  private:
    std::array<Tile, MAP_SIZE * MAP_SIZE> m_tilemap;
  };
}// namespace tiles
}// namespace gfx
