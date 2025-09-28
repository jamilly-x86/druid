module;

#include <cstdint>
#include <limits>

export module druid.graphics.color;

namespace druid::graphics
{
	export struct Rgba
	{
		static constexpr auto Max = std::numeric_limits<uint8_t>::max();

		uint8_t r{};
		uint8_t g{};
		uint8_t b{};
		uint8_t a{Rgba::Max};
	};

	export struct RgbaF
	{
		static constexpr auto Max = 1.0F;
		float r{};
		float g{};
		float b{};
		float a{RgbaF::Max};
	};

	/// \brief This class represents color as 8-bit RGBA.
	export class Color
	{
	public:
		static const Color Druid;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color White;
		static const Color Black;
		static const Color Transparent;

		constexpr Color() = default;
		constexpr Color(const Rgba& x) : rgba{x}
		{
		}

		[[nodiscard]] auto red() const -> uint8_t
		{
			return this->rgba.r;
		}

		[[nodiscard]] auto redF() const -> float
		{
			return static_cast<float>(this->rgba.r) * Color::Scalar;
		}

		[[nodiscard]] auto green() const -> uint8_t
		{
			return this->rgba.g;
		}

		[[nodiscard]] auto greenF() const -> float
		{
			return static_cast<float>(this->rgba.g) * Color::Scalar;
		}

		[[nodiscard]] auto blue() const -> uint8_t
		{
			return this->rgba.b;
		}

		[[nodiscard]] auto blueF() const -> float
		{
			return static_cast<float>(this->rgba.b) * Color::Scalar;
		}

		[[nodiscard]] auto alpha() const -> uint8_t
		{
			return this->rgba.a;
		}

		[[nodiscard]] auto alphaF() const -> float
		{
			return static_cast<float>(this->rgba.a) * Color::Scalar;
		}

		[[nodiscard]] auto rgbaF() const -> RgbaF
		{
			return {.r = this->redF(), .g = this->greenF(), .b = this->blueF(), .a = this->alphaF()};
		}

	private:
		static constexpr auto Scalar = 1.0F / Rgba::Max;
		Rgba rgba{};
	};

	constexpr Color Color::Druid{{.r = 53, .g = 39, .b = 39}};
	constexpr Color Color::Red{{.r = Rgba::Max}};
	constexpr Color Color::Green{{.g = Rgba::Max}};
	constexpr Color Color::Blue{{.b = Rgba::Max}};
	constexpr Color Color::White{{.r = Rgba::Max, .g = Rgba::Max, .b = Rgba::Max}};
	constexpr Color Color::Black{{}};
	constexpr Color Color::Transparent{{.a = 0U}};
}