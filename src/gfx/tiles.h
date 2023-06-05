#pragma once
#include <Color.hpp>
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

    enum class Position {
      Ground,
      Wall,
      Ceiling,
    };

    Tile();
    Tile(Type, Position);
    Tile(Type);
    Tile(Position);

    ~Tile() = default;

    Type type() const { return m_type; }
    Position position() const { return m_position; }

    void set_type(Type);
    void set_position(Position);
    void set_data(Type, Position);


    [[nodiscard]] raylib::Color colour() const { return m_colour; };

  private:
    void update_colour();

    Type m_type = Type::Stone;
    Position m_position = Position::Ground;
    raylib::Color m_colour = raylib::Color(10, 10, 10);
  };

  class Map {
  public:
    Map();
    ~Map() = default;

    const auto& tiles() const { return m_tilemap; }

  private:
    std::array<Tile, MAP_SIZE * MAP_SIZE> m_tilemap;
  };
}// namespace tiles
}// namespace gfx
