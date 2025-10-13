#pragma once

#include <type_traits>

namespace runestone
{
	template <typename Enum>
	class EnumMask
	{
		static_assert(std::is_enum_v<Enum>, "EnumMask<T>: T must be an enum type");

	public:
		using UnderlyingType = std::underlying_type_t<Enum>;

		constexpr EnumMask() noexcept = default;

		constexpr explicit EnumMask(Enum value) noexcept : value_(static_cast<UnderlyingType>(value))
		{
		}

		constexpr explicit EnumMask(UnderlyingType value) noexcept : value_(value)
		{
		}

		constexpr auto operator|(Enum rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ | static_cast<UnderlyingType>(rhs));
		}
		constexpr auto operator|(EnumMask rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ | rhs.value_);
		}

		constexpr auto operator&(Enum rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ & static_cast<UnderlyingType>(rhs));
		}

		constexpr auto operator&(EnumMask rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ & rhs.value_);
		}

		constexpr auto operator^(Enum rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ ^ static_cast<UnderlyingType>(rhs));
		}

		constexpr auto operator^(EnumMask rhs) const noexcept -> EnumMask
		{
			return EnumMask(value_ ^ rhs.value_);
		}

		constexpr auto operator~() const noexcept -> EnumMask
		{
			return EnumMask(~value_);
		}

		constexpr auto operator|=(Enum rhs) noexcept -> EnumMask&
		{
			value_ |= static_cast<UnderlyingType>(rhs);
			return *this;
		}

		constexpr auto operator&=(Enum rhs) noexcept -> EnumMask&
		{
			value_ &= static_cast<UnderlyingType>(rhs);
			return *this;
		}

		constexpr auto operator^=(Enum rhs) noexcept -> EnumMask&
		{
			value_ ^= static_cast<UnderlyingType>(rhs);
			return *this;
		}

		[[nodiscard]] constexpr auto test(Enum flag) const noexcept -> bool
		{
			return (value_ & static_cast<UnderlyingType>(flag)) != 0;
		}

		[[nodiscard]] constexpr auto raw() const noexcept -> UnderlyingType
		{
			return value_;
		}

	private:
		UnderlyingType value_ = 0;
	};


	// allow Enum | Enum
	template <typename Enum>
	constexpr EnumMask<Enum> operator|(Enum lhs, Enum rhs) noexcept
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(static_cast<U>(lhs) | static_cast<U>(rhs));
	}

	// allow Enum & Enum
	template <typename Enum>
	constexpr EnumMask<Enum> operator&(Enum lhs, Enum rhs) noexcept
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(static_cast<U>(lhs) & static_cast<U>(rhs));
	}

	// allow Enum ^ Enum
	template <typename Enum>
	constexpr EnumMask<Enum> operator^(Enum lhs, Enum rhs) noexcept
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
	}

	// allow ~Enum
	template <typename Enum>
	constexpr EnumMask<Enum> operator~(Enum rhs) noexcept
	{
		using U = std::underlying_type_t<Enum>;
		return EnumMask<Enum>(~static_cast<U>(rhs));
	}
}