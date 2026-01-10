module;

#include <cmath>

export module druid.math.Vec3;

export namespace druid::math
{
	/// @struct Vec3
	/// @brief 3D vector with x, y, and z components.
	struct Vec3
	{
		float x{0.0F};
		float y{0.0F};
		float z{0.0F};

		constexpr auto operator+(const Vec3& other) const -> Vec3
		{
			return {x + other.x, y + other.y, z + other.z};
		}

		constexpr auto operator-(const Vec3& other) const -> Vec3
		{
			return {x - other.x, y - other.y, z - other.z};
		}

		constexpr auto operator*(float scalar) const -> Vec3
		{
			return {x * scalar, y * scalar, z * scalar};
		}

		constexpr auto operator/(float scalar) const -> Vec3
		{
			return {x / scalar, y / scalar, z / scalar};
		}

		constexpr auto operator+=(const Vec3& other) -> Vec3&
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		constexpr auto operator-=(const Vec3& other) -> Vec3&
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		constexpr auto operator*=(float scalar) -> Vec3&
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		constexpr auto operator/=(float scalar) -> Vec3&
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		[[nodiscard]] constexpr auto length() const -> float
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		[[nodiscard]] constexpr auto normalize() const -> Vec3
		{
			const auto len = length();
			if (len > 0.0F)
			{
				return {x / len, y / len, z / len};
			}
			return *this;
		}

		[[nodiscard]] constexpr auto dot(const Vec3& other) const -> float
		{
			return x * other.x + y * other.y + z * other.z;
		}

		[[nodiscard]] constexpr auto cross(const Vec3& other) const -> Vec3
		{
			return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
		}
	};
}
