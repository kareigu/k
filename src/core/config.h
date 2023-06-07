#pragma once
#include <expected>
#include <result.hpp>
#include <string>

namespace gfx {
class Renderer;
}

namespace core {
constexpr int DEFAULT_SCREEN_WIDTH = 800;
constexpr int DEFAULT_SCREEN_HEIGHT = 600;
constexpr int DEFAULT_TARGET_FPS = 60;


struct Resolution {
  int width = DEFAULT_SCREEN_WIDTH;
  int height = DEFAULT_SCREEN_HEIGHT;
  int target_fps = DEFAULT_TARGET_FPS;
};

class Config {
public:
  enum class Error {
    HelpShown,
    InvalidArguments,
    Unknown,
  };

  static cpp::result<Config, Error> create_from_args(int argc, char** argv);
  ~Config() = default;

  Resolution resolution() const { return m_resolution; }
  int screen_width() const { return resolution().width; }
  int screen_height() const { return resolution().height; }
  int target_fps() const { return resolution().target_fps; }
  bool debug() const { return m_debug_view; }

  void set_debug(bool enabled = true) { m_debug_view = enabled; }

private:
  Config() = default;
  Resolution m_resolution{};
#ifndef NDEBUG
  bool m_debug_view = true;
#else
  bool m_debug_view = false;
#endif


  friend class gfx::Renderer;
};
}// namespace core
