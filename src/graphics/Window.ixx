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

export module druid.graphics.Window;

import druid.core.Engine;
import druid.core.Event;
import druid.graphics.Color;
import druid.graphics.Node;
import druid.graphics.Renderer;
import druid.graphics.renderer.Raylib;

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

export namespace druid::graphics
{
	/// @class Window
	/// @brief Graphics service responsible for window management and rendering.
	///
	/// `Window` is an engine `Service` that typically:
	/// - Creates and owns the graphics window and rendering backend (`Renderer`).
	/// - Drives per-frame rendering during `update()` and end-of-frame presentation
	///   during `update_end()`.
	/// - Provides a root scene-graph `Node` as the entry point for rendering content.
	///
	/// The exact backend implementation (e.g., Raylib/rlgl) is encapsulated by
	/// `Renderer` and configured internally by this service.
	class Window : public druid::core::Service
	{
	public:
		/// @brief Default window title.
		static constexpr auto DefaultTitle{"Druid"};
		/// @brief Default window width in pixels.
		static constexpr auto DefaultWidth{1280};
		/// @brief Default window height in pixels.
		static constexpr auto DefaultHeight{720};

		/// @brief Construct the window service and bind it to the engine.
		///
		/// Implementations typically allocate the renderer, open a native window,
		/// and initialize any graphics state required for drawing.
		///
		/// @param x Owning engine instance.
		explicit Window(druid::core::Engine& x) : Service{x}, root_node_{}, renderer_{std::make_unique<renderer::Raylib>()}
		{
			InitWindow(width_, height_, title_.c_str());
			SetWindowState(FLAG_WINDOW_RESIZABLE);
		}

		/// @brief Destroy the window service.
		///
		/// Implementations should release renderer resources and close the window.
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
		/// Typically performs per-frame window polling (input/events), prepares
		/// the renderer, and triggers draw traversal starting from `root_node()`.
		auto update(std::chrono::steady_clock::duration /*x*/) -> void override
		{
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
		/// Typically finalizes rendering for the frame (e.g., present/swap buffers),
		/// and performs any end-of-frame housekeeping.
		auto update_end() -> void override
		{
			// Render the scene
			if (renderer_)
			{
				renderer_->begin(Color::Black);
				root_node_.draw(*renderer_);
				renderer_->end();
			}
		}

		/// @brief Get the root node of the window's scene graph.
		///
		/// Rendering typically traverses from this node. Users can attach child
		/// nodes to build a renderable hierarchy.
		///
		/// @return Reference to the root node.
		[[nodiscard]] auto root_node() -> Node&
		{
			return root_node_;
		}

		/// @brief Set the window title.
		/// @param x New title string.
		auto set_title(std::string_view x) -> void
		{
			title_ = x;
			SetWindowTitle(title_.c_str());
		}

		/// @brief Get the current window title.
		/// @return Current title string view.
		[[nodiscard]] auto get_title() -> std::string_view
		{
			return title_;
		}

	private:
		Node root_node_;
		std::unique_ptr<Renderer> renderer_;
		std::string title_{DefaultTitle};
		int width_{DefaultWidth};
		int height_{DefaultHeight};
	};
}
