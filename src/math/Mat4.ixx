export module druid.math.Mat4;

import std;
import druid.math.Vec3;

export namespace druid::math
{
	/// @struct Mat4
	/// @brief 4x4 matrix for 3D transformations (column-major).
	struct Mat4
	{
		// Column-major storage: m[column][row]
		float m[4][4]{};

		constexpr Mat4()
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] = 0.0F;
				}
			}
		}

		/// @brief Create an identity matrix.
		[[nodiscard]] static constexpr auto identity() -> Mat4
		{
			Mat4 result;
			result.m[0][0] = 1.0F;
			result.m[1][1] = 1.0F;
			result.m[2][2] = 1.0F;
			result.m[3][3] = 1.0F;
			return result;
		}

		/// @brief Matrix multiplication.
		[[nodiscard]] constexpr auto operator*(const Mat4& other) const -> Mat4
		{
			Mat4 result;
			for (int col = 0; col < 4; ++col)
			{
				for (int row = 0; row < 4; ++row)
				{
					result.m[col][row] = 0.0F;
					for (int k = 0; k < 4; ++k)
					{
						result.m[col][row] += m[k][row] * other.m[col][k];
					}
				}
			}
			return result;
		}

		/// @brief Access column operator (for glm compatibility: mat[col][row]).
		[[nodiscard]] constexpr auto operator[](int col) -> float*
		{
			return m[col];
		}

		/// @brief Access column operator (const).
		[[nodiscard]] constexpr auto operator[](int col) const -> const float*
		{
			return m[col];
		}

		/// @brief Create a translation matrix.
		[[nodiscard]] static constexpr auto translate(const Vec3& v) -> Mat4
		{
			Mat4 result = identity();
			result.m[3][0] = v.x;
			result.m[3][1] = v.y;
			result.m[3][2] = v.z;
			return result;
		}

		/// @brief Create a scale matrix.
		[[nodiscard]] static constexpr auto scale(const Vec3& v) -> Mat4
		{
			Mat4 result = identity();
			result.m[0][0] = v.x;
			result.m[1][1] = v.y;
			result.m[2][2] = v.z;
			return result;
		}

		/// @brief Create a rotation matrix around an arbitrary axis.
		[[nodiscard]] static auto rotate(float angle_radians, const Vec3& axis) -> Mat4
		{
			const auto c = std::cos(angle_radians);
			const auto s = std::sin(angle_radians);
			const auto t = 1.0F - c;

			const auto normalized = axis.normalize();
			const auto x = normalized.x;
			const auto y = normalized.y;
			const auto z = normalized.z;

			Mat4 result = identity();
			result.m[0][0] = t * x * x + c;
			result.m[0][1] = t * x * y + s * z;
			result.m[0][2] = t * x * z - s * y;

			result.m[1][0] = t * x * y - s * z;
			result.m[1][1] = t * y * y + c;
			result.m[1][2] = t * y * z + s * x;

			result.m[2][0] = t * x * z + s * y;
			result.m[2][1] = t * y * z - s * x;
			result.m[2][2] = t * z * z + c;

			return result;
		}

		/// @brief Get pointer to matrix data (for passing to C APIs).
		[[nodiscard]] constexpr auto data() -> float*
		{
			return &m[0][0];
		}

		/// @brief Get pointer to matrix data (const).
		[[nodiscard]] constexpr auto data() const -> const float*
		{
			return &m[0][0];
		}
	};

	/// @brief Convert degrees to radians.
	[[nodiscard]] constexpr auto radians(float degrees) -> float
	{
		return degrees * 3.14159265358979323846F / 180.0F;
	}
}
