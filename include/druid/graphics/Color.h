#pragma once

#include <cstdint>
#include <limits>

namespace druid::graphics
{
	/// @struct Rgba
	/// @brief 8-bit per-channel RGBA color representation.
	///
	/// Stores red, green, blue, and alpha channels as unsigned 8-bit integers.
	/// This format is commonly used for CPU-side color storage and low-level
	/// graphics APIs.
	struct Rgba
	{
		/// @brief Maximum value of an 8-bit color channel (255).
		static constexpr auto Max = std::numeric_limits<uint8_t>::max();
		/// @brief Red channel.
		uint8_t r{};
		/// @brief Green channel.
		uint8_t g{};
		/// @brief Blue channel.
		uint8_t b{};
		/// @brief Alpha channel (opacity).
		uint8_t a{Rgba::Max};
	};

	/// @struct RgbaF
	/// @brief Floating-point RGBA color representation.
	///
	/// Stores red, green, blue, and alpha channels as normalized floating-point
	/// values in the range [0.0, 1.0].
	struct RgbaF
	{
		/// @brief Maximum normalized channel value (1.0).
		static constexpr auto Max = 1.0F;
		/// @brief Red channel (normalized).
		float r{};
		/// @brief Green channel (normalized).
		float g{};
		/// @brief Blue channel (normalized).
		float b{};
		/// @brief Alpha channel (opacity, normalized).
		float a{RgbaF::Max};
	};

	/// @class Color
	/// @brief High-level color abstraction using 8-bit RGBA storage.
	///
	/// `Color` wraps an `Rgba` value and provides convenient accessors for both
	/// integer and normalized floating-point representations. This allows a
	/// single color type to be used across CPU-side logic and GPU-facing APIs.
	class Color
	{
	public:
		/// @brief Predefined Druid theme color.
		static const Color Druid;
		/// @brief Predefined red color.
		static const Color Red;
		/// @brief Predefined green color.
		static const Color Green;
		/// @brief Predefined blue color.
		static const Color Blue;
		/// @brief Predefined white color.
		static const Color White;
		/// @brief Predefined black color.
		static const Color Black;
		/// @brief Fully transparent color.
		static const Color Transparent;

		/// @brief Default constructor (black, fully opaque).
		constexpr Color() noexcept = default;

		/// @brief Construct a color from an 8-bit RGBA value.
		/// @param x RGBA color value to wrap.
		constexpr Color(const Rgba& x) noexcept : rgba_{x}
		{
		}

		/// @brief Get the red channel as an 8-bit integer.
		/// @return Red channel value in [0, 255].
		[[nodiscard]] auto red() const -> uint8_t;

		/// @brief Get the red channel as a normalized float.
		/// @return Red channel value in [0.0, 1.0].
		[[nodiscard]] auto red_f() const -> float;

		/// @brief Get the green channel as an 8-bit integer.
		/// @return Green channel value in [0, 255].
		[[nodiscard]] auto green() const -> uint8_t;

		/// @brief Get the green channel as a normalized float.
		/// @return Green channel value in [0.0, 1.0].
		[[nodiscard]] auto green_f() const -> float;

		/// @brief Get the blue channel as an 8-bit integer.
		/// @return Blue channel value in [0, 255].
		[[nodiscard]] auto blue() const -> uint8_t;

		/// @brief Get the blue channel as a normalized float.
		/// @return Blue channel value in [0.0, 1.0].
		[[nodiscard]] auto blue_f() const -> float;

		/// @brief Get the alpha channel as an 8-bit integer.
		/// @return Alpha channel value in [0, 255].
		[[nodiscard]] auto alpha() const -> uint8_t;

		/// @brief Get the alpha channel as a normalized float.
		/// @return Alpha channel value in [0.0, 1.0].
		[[nodiscard]] auto alpha_f() const -> float;

		/// @brief Get the full color as a normalized floating-point RGBA value.
		/// @return RGBA color with normalized components.
		[[nodiscard]] auto rgba_f() const -> RgbaF;

	private:
		/// @brief Scalar used to convert 8-bit channels to normalized floats.
		static constexpr auto Scalar = 1.0F / Rgba::Max;
		/// @brief Underlying 8-bit RGBA storage.
		Rgba rgba_{};
	};
}
