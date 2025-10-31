#pragma once

#include <cstdint>
#include <variant>

namespace druid::core
{
	struct EventKeyboard
	{
		enum class Type : std::uint8_t
		{
			None,
			KeyPressed,
			KeyReleased
		};

		enum class Key : std::uint8_t
		{
			None,
			Apostrophe,
			Comma,
			Minus,
			Period,
			Slash,
			Zero,
			One,
			Two,
			Three,
			Four,
			Five,
			Six,
			Seven,
			Eight,
			Nine,
			Semicolon,
			Equal,
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			LeftBracket,
			Backslash,
			RightBracket,
			Grave,

			// Function keys
			Space,
			Escape,
			Enter,
			Tab,
			Backspace,
			Insert,
			Delete,
			Right,
			Left,
			Down,
			Up,
			PageUp,
			PageDown,
			Home,
			End,
			CapsLock,
			ScrollLock,
			NumLock,
			PrintScreen,
			Pause,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			LeftShift,
			LeftControl,
			LeftAlt,
			LeftSuper,
			RightShift,
			RightControl,
			RightAlt,
			RightSuper,
			KbMenu,

			// Keypad keys
			Kp0,
			Kp1,
			Kp2,
			Kp3,
			Kp4,
			Kp5,
			Kp6,
			Kp7,
			Kp8,
			Kp9,
			KpDecimal,
			KpDivide,
			KpMultiply,
			KpSubtract,
			KpAdd,
			KpEnter,
			KpEqual,

			// Android key button
			Back,
			Menu,
			VolumeUp,
			VolumeDown
		};

		Type type{Type::None};
		Key key{Key::None};
	};

	struct EventWindow
	{
		enum class Type : std::uint8_t
		{
			None,
			Moved,
			Resized,
			Closed
		};

		Type type{Type::None};
	};

	struct EventMouse
	{
		enum class Type : std::uint8_t
		{
			None,
			ButtonPressed,
			ButtonReleased,
			Moved
		};

		enum class Button : std::uint8_t
		{
			None,
			Left,
			Right,
			Middle
		};

		Type type{Type::None};
		Button button{Button::None};
	};

	using Event = std::variant<std::monostate, EventKeyboard, EventWindow, EventMouse>;
}