#pragma once

#include <cstdint>
#include <limits>

namespace druid::graphics
{
	struct Rgba
	{
		static constexpr auto Max = std::numeric_limits<uint8_t>::max();

		uint8_t r{};
		uint8_t g{};
		uint8_t b{};
		uint8_t a{Rgba::Max};
	};

	struct RgbaF
	{
		static constexpr auto Max = 1.0F;
		float r{};
		float g{};
		float b{};
		float a{RgbaF::Max};
	};

	/// \brief This class represents color as 8-bit RGBA.
	class Color
	{
	public:
		static const Color Druid;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color White;
		static const Color Black;
		static const Color Transparent;

		constexpr Color() noexcept = default;
		constexpr Color(const Rgba& x) noexcept : rgba_{x}
		{
		}

		[[nodiscard]] auto red() const -> uint8_t;

		[[nodiscard]] auto red_f() const -> float;

		[[nodiscard]] auto green() const -> uint8_t;

		[[nodiscard]] auto green_f() const -> float;

		[[nodiscard]] auto blue() const -> uint8_t;

		[[nodiscard]] auto blue_f() const -> float;

		[[nodiscard]] auto alpha() const -> uint8_t;

		[[nodiscard]] auto alpha_f() const -> float;

		[[nodiscard]] auto rgba_f() const -> RgbaF;

	private:
		static constexpr auto Scalar = 1.0F / Rgba::Max;
		Rgba rgba_{};
	};
}