#pragma once

#include <string>

namespace druid::raylib
{
	/// @struct Window
	/// @brief Lightweight window description for Raylib-backed rendering.
	///
	/// Represents the basic properties required to create or configure a native
	/// window using a Raylib-based backend. This structure is typically used as a
	/// plain data container and does not manage any window lifetime or resources.
	struct Window
	{
		/// @brief Window title displayed in the title bar.
		std::string title;
		/// @brief X-coordinate of the window position (in screen space).
		int x{};
		/// @brief Y-coordinate of the window position (in screen space).
		int y{};
		/// @brief Width of the window in pixels.
		int width{};
		/// @brief Height of the window in pixels.
		int height{};
	};
}
