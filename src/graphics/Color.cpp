#include <druid/graphics/Color.h>

namespace druid::graphics
{
	const Color Color::Druid{{.r = 53, .g = 39, .b = 39}};
	const Color Color::Red{{.r = Rgba::Max}};
	const Color Color::Green{{.g = Rgba::Max}};
	const Color Color::Blue{{.b = Rgba::Max}};
	const Color Color::White{{.r = Rgba::Max, .g = Rgba::Max, .b = Rgba::Max}};
	const Color Color::Black{{}};
	const Color Color::Transparent{{.a = 0U}};

	auto Color::red() const -> uint8_t
	{
		return rgba_.r;
	}

	auto Color::red_f() const -> float
	{
		return static_cast<float>(rgba_.r) * Color::Scalar;
	}

	auto Color::green() const -> uint8_t
	{
		return rgba_.g;
	}

	auto Color::green_f() const -> float
	{
		return static_cast<float>(rgba_.g) * Color::Scalar;
	}

	auto Color::blue() const -> uint8_t
	{
		return rgba_.b;
	}

	auto Color::blue_f() const -> float
	{
		return static_cast<float>(rgba_.b) * Color::Scalar;
	}

	auto Color::alpha() const -> uint8_t
	{
		return rgba_.a;
	}

	auto Color::alpha_f() const -> float
	{
		return static_cast<float>(rgba_.a) * Color::Scalar;
	}

	auto Color::rgba_f() const -> RgbaF
	{
		return {.r = this->red_f(), .g = this->green_f(), .b = this->blue_f(), .a = this->alpha_f()};
	}
}