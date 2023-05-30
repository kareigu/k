#pragma once
#include <expected>
#include <string>

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

  static std::expected<Config, Error> create_from_args(int argc, char** argv);
  ~Config() = default;

  Resolution resolution() const { return m_resolution; }
  int screen_width() const { return resolution().width; }
  int screen_height() const { return resolution().height; }
  int target_fps() const { return resolution().target_fps; }

private:
  Config() = default;
  Resolution m_resolution{};
};
}// namespace core
