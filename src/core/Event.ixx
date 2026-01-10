module;

#include <cstdint>

export module druid.core.Event;

import std;

export namespace druid::core
{
	/// @struct EventKeyboard
	/// @brief Keyboard event payload for the engine event system.
	///
	/// Represents a keyboard-related event, including the event category (`type`)
	/// and the specific key involved (`key`).
	struct EventKeyboard
	{
		/// @enum Type
		/// @brief Kind of keyboard event.
		enum class Type : std::uint8_t
		{
			/// @brief No keyboard event / default value.
			None,
			/// @brief A key was pressed.
			KeyPressed,
			/// @brief A key was released.
			KeyReleased
		};

		/// @enum Key
		/// @brief Canonical key identifiers used by the engine.
		///
		/// These values are intended to be platform-agnostic. Platform/back-end code
		/// should translate native key codes into this enum before emitting events.
		enum class Key : std::uint8_t
		{
			/// @cond DOXYGEN_SHOULD_SKIP_THIS
			/// @brief No key / unknown key.
			None,

			// Punctuation / symbols
			Apostrophe,
			Comma,
			Minus,
			Period,
			Slash,

			// Number row
			// clang-format off
			Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
			// clang-format on

			// More punctuation / symbols
			Semicolon,
			Equal,

			// Letters
			// clang-format off
			A, B, C, D, E, F, G, H, I, J, K, L, M,
			N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			// clang-format on

			LeftBracket,
			Backslash,
			RightBracket,
			Grave,

			// Function / navigation keys
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
			// clang-format off
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			// clang-format on

			// Modifiers / system keys
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
			// clang-format off
			Kp0, Kp1, Kp2, Kp3, Kp4, Kp5, Kp6, Kp7, Kp8, Kp9,
			// clang-format on
			KpDecimal,
			KpDivide,
			KpMultiply,
			KpSubtract,
			KpAdd,
			KpEnter,
			KpEqual,

			// Android key buttons (or platform-specific equivalents)
			Back,
			Menu,
			VolumeUp,
			VolumeDown
			/// @endcond
		};

		/// @brief Keyboard event type (pressed/released).
		Type type{Type::None};
		/// @brief Key associated with the event.
		Key key{Key::None};
	};

	/// @struct EventWindow
	/// @brief Window event payload for the engine event system.
	///
	/// Represents window lifecycle and window geometry events (move/resize/close).
	struct EventWindow
	{
		/// @enum Type
		/// @brief Kind of window event.
		enum class Type : std::uint8_t
		{
			/// @brief No window event / default value.
			None,
			/// @brief The window moved.
			Moved,
			/// @brief The window resized.
			Resized,
			/// @brief The window was requested to close / closed.
			Closed
		};

		/// @brief Window event type.
		Type type{Type::None};
	};

	/// @struct EventMouse
	/// @brief Mouse event payload for the engine event system.
	///
	/// Represents mouse button presses/releases and mouse motion events.
	struct EventMouse
	{
		/// @enum Type
		/// @brief Kind of mouse event.
		enum class Type : std::uint8_t
		{
			/// @brief No mouse event / default value.
			None,
			/// @brief A mouse button was pressed.
			ButtonPressed,
			/// @brief A mouse button was released.
			ButtonReleased,
			/// @brief The mouse moved.
			Moved
		};

		/// @enum Button
		/// @brief Mouse button identifiers.
		enum class Button : std::uint8_t
		{
			/// @brief No button / not applicable.
			None,
			/// @brief Left mouse button.
			Left,
			/// @brief Right mouse button.
			Right,
			/// @brief Middle mouse button / wheel click.
			Middle
		};

		/// @brief Mouse event type.
		Type type{Type::None};
		/// @brief Button associated with the event (if applicable).
		Button button{Button::None};
	};

	/// @brief Unified engine event type.
	///
	/// The engine uses a tagged union (`std::variant`) to represent events from
	/// multiple domains in a single type. `std::monostate` represents "no event".
	///
	/// Typical usage:
	/// @code
	/// std::visit(Engine::Overloaded{
	///   [](std::monostate){},
	///   [](const EventKeyboard& e){ /* ... */ },
	///   [](const EventWindow& e){ /* ... */ },
	///   [](const EventMouse& e){ /* ... */ },
	/// }, event);
	/// @endcode
	using Event = std::variant<std::monostate, EventKeyboard, EventWindow, EventMouse>;
}
