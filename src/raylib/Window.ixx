module;

#include <chrono>

// Need to define a large max range for magic_enum
// to support all of the raylib keys.
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define MAGIC_ENUM_RANGE_MIN 0

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define MAGIC_ENUM_RANGE_MAX 400
#include <raylib.h>
#include <magic_enum/magic_enum.hpp>

export module druid.raylib.Window;

import druid.core.Engine;
import druid.core.Event;
import druid.graphics.Color;
import druid.graphics.Node;
import druid.graphics.Renderer;
import druid.raylib.Renderer;
import druid.graphics.Window;

namespace
{
	[[nodiscard]] auto ConvertRayLibKey(KeyboardKey x) -> druid::core::EventKeyboard::Key
	{
		using Key = druid::core::EventKeyboard::Key;

		switch (x)
		{
			case KEY_NULL:
				return Key::None;
			case KEY_APOSTROPHE:
				return Key::Apostrophe;
			case KEY_COMMA:
				return Key::Comma;
			case KEY_MINUS:
				return Key::Minus;
			case KEY_PERIOD:
				return Key::Period;
			case KEY_SLASH:
				return Key::Slash;
			case KEY_ZERO:
				return Key::Zero;
			case KEY_ONE:
				return Key::One;
			case KEY_TWO:
				return Key::Two;
			case KEY_THREE:
				return Key::Three;
			case KEY_FOUR:
				return Key::Four;
			case KEY_FIVE:
				return Key::Five;
			case KEY_SIX:
				return Key::Six;
			case KEY_SEVEN:
				return Key::Seven;
			case KEY_EIGHT:
				return Key::Eight;
			case KEY_NINE:
				return Key::Nine;
			case KEY_SEMICOLON:
				return Key::Semicolon;
			case KEY_EQUAL:
				return Key::Equal;
			case KEY_A:
				return Key::A;
			case KEY_B:
				return Key::B;
			case KEY_C:
				return Key::C;
			case KEY_D:
				return Key::D;
			case KEY_E:
				return Key::E;
			case KEY_F:
				return Key::F;
			case KEY_G:
				return Key::G;
			case KEY_H:
				return Key::H;
			case KEY_I:
				return Key::I;
			case KEY_J:
				return Key::J;
			case KEY_K:
				return Key::K;
			case KEY_L:
				return Key::L;
			case KEY_M:
				return Key::M;
			case KEY_N:
				return Key::N;
			case KEY_O:
				return Key::O;
			case KEY_P:
				return Key::P;
			case KEY_Q:
				return Key::Q;
			case KEY_R:
				return Key::R;
			case KEY_S:
				return Key::S;
			case KEY_T:
				return Key::T;
			case KEY_U:
				return Key::U;
			case KEY_V:
				return Key::V;
			case KEY_W:
				return Key::W;
			case KEY_X:
				return Key::X;
			case KEY_Y:
				return Key::Y;
			case KEY_Z:
				return Key::Z;
			case KEY_LEFT_BRACKET:
				return Key::LeftBracket;
			case KEY_BACKSLASH:
				return Key::Backslash;
			case KEY_RIGHT_BRACKET:
				return Key::RightBracket;
			case KEY_GRAVE:
				return Key::Grave;
			case KEY_SPACE:
				return Key::Space;
			case KEY_ESCAPE:
				return Key::Escape;
			case KEY_ENTER:
				return Key::Enter;
			case KEY_TAB:
				return Key::Tab;
			case KEY_BACKSPACE:
				return Key::Backspace;
			case KEY_INSERT:
				return Key::Insert;
			case KEY_DELETE:
				return Key::Delete;
			case KEY_RIGHT:
				return Key::Right;
			case KEY_LEFT:
				return Key::Left;
			case KEY_DOWN:
				return Key::Down;
			case KEY_UP:
				return Key::Up;
			case KEY_PAGE_UP:
				return Key::PageUp;
			case KEY_PAGE_DOWN:
				return Key::PageDown;
			case KEY_HOME:
				return Key::Home;
			case KEY_END:
				return Key::End;
			case KEY_CAPS_LOCK:
				return Key::CapsLock;
			case KEY_SCROLL_LOCK:
				return Key::ScrollLock;
			case KEY_NUM_LOCK:
				return Key::NumLock;
			case KEY_PRINT_SCREEN:
				return Key::PrintScreen;
			case KEY_PAUSE:
				return Key::Pause;
			case KEY_F1:
				return Key::F1;
			case KEY_F2:
				return Key::F2;
			case KEY_F3:
				return Key::F3;
			case KEY_F4:
				return Key::F4;
			case KEY_F5:
				return Key::F5;
			case KEY_F6:
				return Key::F6;
			case KEY_F7:
				return Key::F7;
			case KEY_F8:
				return Key::F8;
			case KEY_F9:
				return Key::F9;
			case KEY_F10:
				return Key::F10;
			case KEY_F11:
				return Key::F11;
			case KEY_F12:
				return Key::F12;
			case KEY_LEFT_SHIFT:
				return Key::LeftShift;
			case KEY_LEFT_CONTROL:
				return Key::LeftControl;
			case KEY_LEFT_ALT:
				return Key::LeftAlt;
			case KEY_LEFT_SUPER:
				return Key::LeftSuper;
			case KEY_RIGHT_SHIFT:
				return Key::RightShift;
			case KEY_RIGHT_CONTROL:
				return Key::RightControl;
			case KEY_RIGHT_ALT:
				return Key::RightAlt;
			case KEY_RIGHT_SUPER:
				return Key::RightSuper;
			case KEY_KB_MENU:
				return Key::KbMenu;
			case KEY_KP_0:
				return Key::Kp0;
			case KEY_KP_1:
				return Key::Kp1;
			case KEY_KP_2:
				return Key::Kp2;
			case KEY_KP_3:
				return Key::Kp3;
			case KEY_KP_4:
				return Key::Kp4;
			case KEY_KP_5:
				return Key::Kp5;
			case KEY_KP_6:
				return Key::Kp6;
			case KEY_KP_7:
				return Key::Kp7;
			case KEY_KP_8:
				return Key::Kp8;
			case KEY_KP_9:
				return Key::Kp9;
			case KEY_KP_DECIMAL:
				return Key::KpDecimal;
			case KEY_KP_DIVIDE:
				return Key::KpDivide;
			case KEY_KP_MULTIPLY:
				return Key::KpMultiply;
			case KEY_KP_SUBTRACT:
				return Key::KpSubtract;
			case KEY_KP_ADD:
				return Key::KpAdd;
			case KEY_KP_ENTER:
				return Key::KpEnter;
			case KEY_KP_EQUAL:
				return Key::KpEqual;
			case KEY_BACK:
				return Key::Back;
			case KEY_MENU:
				return Key::Menu;
			case KEY_VOLUME_UP:
				return Key::VolumeUp;
			case KEY_VOLUME_DOWN:
				return Key::VolumeDown;
			default:
				return Key::None;
		}
	}
}

export namespace druid::raylib
{
	/// @class Window
	/// @brief Raylib-based window implementation.
	///
	/// `Window` provides a concrete implementation of the `druid::graphics::Window` interface
	/// using the raylib library for window creation, input handling, and rendering.
	class Window : public druid::graphics::Window
	{
	public:
		/// @brief Construct the raylib window service and bind it to the engine.
		///
		/// Initializes the raylib window with default dimensions and creates a
		/// raylib-based renderer.
		///
		/// @param x Owning engine instance.
		explicit Window(druid::core::Engine& x) : druid::graphics::Window{x}
		{
			set_renderer(std::make_unique<druid::raylib::Renderer>());
			InitWindow(width_, height_, get_title().data());
			SetWindowState(FLAG_WINDOW_RESIZABLE);
		}

		/// @brief Destroy the raylib window service.
		///
		/// Closes the raylib window and releases all resources.
		~Window() override
		{
			CloseWindow();
		}

		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&&) noexcept -> Window& = delete;

		/// @brief Per-frame update hook.
		///
		/// Polls raylib for window events and input, and dispatches them to the engine
		/// event system.
		auto update(std::chrono::steady_clock::duration /*x*/) -> void override
		{
			// Check if title has changed and update raylib window
			if (get_title() != last_title_)
			{
				last_title_ = get_title();
				SetWindowTitle(last_title_.c_str());
			}

			if (WindowShouldClose())
			{
				using druid::core::EventWindow;
				EventWindow e{.type = EventWindow::Type::Closed};
				engine().event(e);
			}

			const auto keys = magic_enum::enum_values<KeyboardKey>();

			for (auto key : keys)
			{
				using druid::core::EventKeyboard;

				if (IsKeyPressed(key))
				{
					EventKeyboard e{.type = EventKeyboard::Type::KeyPressed, .key = ConvertRayLibKey(key)};
					engine().event(e);
				}
				else if (IsKeyReleased(key))
				{
					EventKeyboard e{.type = EventKeyboard::Type::KeyReleased, .key = ConvertRayLibKey(key)};
					engine().event(e);
				}
			}
		}

		/// @brief End-of-frame hook.
		///
		/// Renders the scene graph using the raylib renderer.
		auto update_end() -> void override
		{
			// Render the scene
			if (auto* renderer = get_renderer())
			{
				renderer->begin(druid::graphics::Color::Black);
				root_node().draw(*renderer);
				renderer->end();
			}
		}

	private:
		std::string last_title_{DefaultTitle};
		int width_{DefaultWidth};
		int height_{DefaultHeight};
	};
}
