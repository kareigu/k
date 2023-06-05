#pragma once
#include "../renderer.h"
#include "raylib.h"
#include <map>
#include <optional>
#include <raylib-cpp.hpp>


namespace core {
namespace input {
  enum class Action {
    Up,
    Down,
    Left,
    Right,
    Confirm,
    Cancel,
#ifndef NDEBUG
    ToggleDebug,
#endif
    EnumSize,
  };

  struct ActionData {
    bool temp;
  };

  template<typename T>
  using DualBind = std::pair<T, std::optional<T>>;

  enum class BindType {
    Press,
    Hold,
  };

  struct Binding {
    BindType type = BindType::Press;
    DualBind<KeyboardKey> keyboard_bind;
    DualBind<GamepadButton> controller_bind;
  };

  class Bindings {
  public:
    Action set_binding(Action action, KeyboardKey key, uint8_t slot);

    const auto& bindings() const { return m_bindings; }

    Bindings();
    ~Bindings() = default;

  private:
    std::map<Action, Binding> m_bindings;
  };

  class Handler {
  public:
    Handler();
    ~Handler() = default;

    const auto& bindings() const { return m_bindings.bindings(); }
    auto& bindings_manager() { return m_bindings; }

    void process_inputs();
    bool handle_inputs(gfx::Renderer::State& state, long long time_diff);

    void add_to_action_queue(Action action);

  private:
    std::vector<Action> m_action_queue;
    Bindings m_bindings;
  };
}// namespace input
}// namespace core
