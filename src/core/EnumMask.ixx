/// @file EnumMask.ixx
/// @module druid.core.enummask
module;

#include <initializer_list>
#include <type_traits>
#include <druid/core/Concepts.h>

export module druid.core.enummask;

export namespace druid::core
{
	/// @class EnumMask
	/// @brief Lightweight bitmask wrapper for enum types.
    ///
	/// Stores a bitmask whose bits correspond to enumerators of a given enum
	/// type `T`. Provides helpers to test, set, clear, flip, and retrieve the
	/// underlying mask.
    ///
	/// @tparam T An enumeration type that satisfies
    ///
	/// @code
	/// enum class Flags : unsigned { A = 1, B = 2, C = 4 };
	/// EnumMask<Flags> m{ Flags::A, Flags::C };
	/// if (m.test(Flags::A)) { /* ... *\/ }
	/// m.set(Flags::B);
	/// auto raw = m.get(); // underlying integer
	/// @endcode
	template <TypeEnum T>
	class EnumMask
	{
	public:
		/// @brief The integer type used to store the bitmask.
		using UnderlyingType = std::underlying_type_t<T>;

		/// @brief Construct an empty mask with all bits cleared.
		constexpr EnumMask() = default;

		/// @brief Construct a mask from a list of enumerators to enable.
		/// @param x Initializers (each enumerator's bit will be set).
		constexpr explicit EnumMask(std::initializer_list<T> x)
		{
			for (auto e : x)
			{
				this->set(e);
			}
		}

		/// @brief Check whether the bit for @p x is set.
		/// @param x The enumerator to test.
		/// @return `true` if the corresponding bit is set; `false` otherwise.
		[[nodiscard]] constexpr auto test(T x) const noexcept -> bool
		{
			return (mask_ & static_cast<UnderlyingType>(x)) != 0x0;
		}

		/// @brief Set (enable) the bit for @p x.
		/// @param x The enumerator whose bit to set.
		constexpr auto set(T x) noexcept -> void
		{
			mask_ |= static_cast<UnderlyingType>(x);
		}

		/// @brief Get the raw integer mask value.
		/// @return The underlying bitmask.
		[[nodiscard]] constexpr auto get() const noexcept -> UnderlyingType
		{
			return mask_;
		}

		/// @brief Clear (disable) the bit for @p x.
		/// @param x The enumerator whose bit to clear.
		constexpr auto remove(T x) noexcept -> void
		{
			mask_ &= ~static_cast<UnderlyingType>(x);
		}

		/// @brief Toggle the bit for @p x.
		/// @param x The enumerator whose bit to toggle.
		constexpr auto flip(T x) noexcept -> void
		{
			mask_ ^= static_cast<UnderlyingType>(x);
		}

		/// @brief Clear all bits (set mask to 0).
		constexpr auto clear() noexcept -> void
		{
			mask_ = 0;
		}

	private:
		UnderlyingType mask_{};
	};
}