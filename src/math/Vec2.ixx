module;

#include <cmath>

export module druid.math.Vec2;

export namespace druid::math
{
	/// @struct Vec2
	/// @brief 2D vector with x and y components.
	struct Vec2
	{
		float x{0.0F};
		float y{0.0F};

		constexpr auto operator+(const Vec2& other) const -> Vec2
		{
			return {x + other.x, y + other.y};
		}

		constexpr auto operator-(const Vec2& other) const -> Vec2
		{
			return {x - other.x, y - other.y};
		}

		constexpr auto operator*(float scalar) const -> Vec2
		{
			return {x * scalar, y * scalar};
		}

		constexpr auto operator/(float scalar) const -> Vec2
		{
			return {x / scalar, y / scalar};
		}

		constexpr auto operator+=(const Vec2& other) -> Vec2&
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		constexpr auto operator-=(const Vec2& other) -> Vec2&
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		constexpr auto operator*=(float scalar) -> Vec2&
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		constexpr auto operator/=(float scalar) -> Vec2&
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		[[nodiscard]] constexpr auto length() const -> float
		{
			return std::sqrt(x * x + y * y);
		}

		[[nodiscard]] constexpr auto normalize() const -> Vec2
		{
			const auto len = length();
			if (len > 0.0F)
			{
				return {x / len, y / len};
			}
			return *this;
		}

		[[nodiscard]] constexpr auto dot(const Vec2& other) const -> float
		{
			return x * other.x + y * other.y;
		}
	};
}
