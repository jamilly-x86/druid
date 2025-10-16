module;

#include <raylib.h>
#include <chrono>

// Need to define a large max range for magic_enum
// to support all of the raylib keys.
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 400
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <string>
#include <string_view>

export module druid.graphics.window;
import druid.core.engine;
import druid.core.event;
import druid.graphics.node;
import druid.graphics.renderer;
import druid.graphics.renderer.raylib;

namespace druid::graphics
{
	export class Window : public druid::core::Service
	{
	public:
		static constexpr auto DefaultTitle{"Druid"};
		static constexpr auto DefaultWidth{1280};
		static constexpr auto DefaultHeight{720};

		Window(druid::core::Engine& x) : Service{x}
		{
			InitWindow(width_, height_, title_.c_str());
			SetWindowState(FLAG_WINDOW_RESIZABLE);
		}

		~Window() override
		{
			CloseWindow();
		}

		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&&) noexcept -> Window& = delete;

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
					EventKeyboard e{.type = EventKeyboard::Type::KeyPressed, .key = convert_raylib_key(key)};
					engine().event(e);
				}
				else if (IsKeyReleased(key))
				{
					EventKeyboard e{.type = EventKeyboard::Type::KeyReleased, .key = convert_raylib_key(key)};
					engine().event(e);
				}
			}
		}

		auto update_end() -> void override
		{
			renderer_->begin(Color::Druid);
			root_node_.draw(*renderer_);
			renderer_->end();
		}

		[[nodiscard]] auto root_node() -> Node&
		{
			return root_node_;
		}

		auto set_title(std::string_view x) -> void
		{
			title_ = x;
			SetWindowTitle(title_.c_str());
		}

		[[nodiscard]] auto get_title() -> std::string_view
		{
			return title_;
		}

		auto resize(int w, int h) -> void
		{
			width_ = w;
			height_ = h;
			SetWindowSize(w, h);
		}

		auto move(int x, int y) -> void
		{
			x_ = x;
			y_ = y;
			SetWindowPosition(x, y);
		}

		[[nodiscard]] auto x() const -> int
		{
			return x_;
		}

		[[nodiscard]] auto y() const -> int
		{
			return y_;
		}

		[[nodiscard]] auto width() const -> int
		{
			return width_;
		}

		[[nodiscard]] auto height() const -> int
		{
			return height_;
		}

		[[nodiscard]] static auto convert_raylib_key(KeyboardKey x) -> druid::core::EventKeyboard::Key
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

	private:
		std::string title_{DefaultTitle};
		int x_{};
		int y_{};
		int width_{DefaultWidth};
		int height_{DefaultHeight};
		Node root_node_;
		std::unique_ptr<Renderer> renderer_{std::make_unique<renderer::Raylib>()};
	};
}