#include "input.h"
#include "../log.h"
#include "assert.h"
#include "raylib.h"
#include <stdexcept>


namespace core {
namespace input {
  Bindings::Bindings() {
    m_bindings = {
            {Action::Up,
             Binding{
                     .type = BindType::Hold,
                     .keyboard_bind = {KEY_UP, KEY_W},
                     .controller_bind = {GAMEPAD_BUTTON_LEFT_FACE_UP, std::nullopt}}},
            {Action::Down,
             Binding{
                     .type = BindType::Hold,
                     .keyboard_bind = {KEY_DOWN, KEY_S},
                     .controller_bind = {GAMEPAD_BUTTON_LEFT_FACE_DOWN, std::nullopt}}},
            {Action::Left,
             Binding{
                     .type = BindType::Hold,
                     .keyboard_bind = {KEY_LEFT, KEY_A},
                     .controller_bind = {GAMEPAD_BUTTON_LEFT_FACE_LEFT, std::nullopt}}},
            {Action::Right,
             Binding{
                     .type = BindType::Hold,
                     .keyboard_bind = {KEY_RIGHT, KEY_D},
                     .controller_bind = {GAMEPAD_BUTTON_LEFT_FACE_RIGHT, std::nullopt}}},
            {Action::Confirm,
             Binding{
                     .keyboard_bind = {KEY_ENTER, KEY_SPACE},
                     .controller_bind = {GAMEPAD_BUTTON_RIGHT_FACE_LEFT, std::nullopt}}},
    };
  }

  Handler::Handler() {
    m_action_queue.resize(static_cast<size_t>(Action::EnumSize));
  }

  void Handler::add_to_action_queue(Action action) {
    m_action_queue.emplace_back(action);
  }

  void Handler::process_inputs() {
    bool controller_on = IsGamepadAvailable(0);

    for (const auto& [action, binding] : bindings()) {
      auto key_fn = binding.type == BindType::Hold ? &IsKeyDown : &IsKeyPressed;

      if (key_fn(binding.keyboard_bind.first)
          || key_fn(binding.keyboard_bind.second.value_or(KEY_NULL)))
        add_to_action_queue(action);

      if (!controller_on)
        continue;

      auto controller_fn = binding.type == BindType::Hold ? &IsGamepadButtonDown : &IsGamepadButtonPressed;

      if (controller_fn(0, binding.controller_bind.first)
          || controller_fn(0, binding.controller_bind.second.value_or(GAMEPAD_BUTTON_UNKNOWN)))
        add_to_action_queue(action);
    }
  }

  bool Handler::handle_inputs(gfx::Renderer::State& state, float time_diff) {
    constexpr float MOVE_MULTIPLIER = 18.0f;
    bool input_handled = false;
    gfx::Renderer::State::CameraOffset new_offset;
    for (const auto& action : m_action_queue) {
      switch (action) {
        case Action::Up:
          new_offset.y += MOVE_MULTIPLIER * time_diff;
          input_handled = true;
          break;
        case Action::Down:
          new_offset.y -= MOVE_MULTIPLIER * time_diff;
          input_handled = true;
          break;
        case Action::Left:
          new_offset.x += MOVE_MULTIPLIER * time_diff;
          input_handled = true;
          break;
        case Action::Right:
          new_offset.x -= MOVE_MULTIPLIER * time_diff;
          input_handled = true;
          break;
        case Action::Confirm:
          log::info("Confirm pressed");
          input_handled = true;
          break;
        case Action::Cancel:
        case Action::ToggleDebug:
        case Action::EnumSize:
          log::warn("Unimplemented");
          break;
      }
    }
    if (new_offset.x != 0.0f || new_offset.y != 0.0f) {
      state.update_camera_offset([new_offset](auto& offset) {
        offset.x += new_offset.x;
        offset.y += new_offset.y;
      });
    }


    m_action_queue.clear();
    return input_handled;
  }
}// namespace input
}// namespace core
