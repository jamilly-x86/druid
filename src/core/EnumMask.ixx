module;

#include <initializer_list>
#include <type_traits>

export module druid.core.enummask;

import druid.core.concepts;

export namespace druid::core
{
	template <TypeEnum T>
	class EnumMask
	{
	public:
		using UnderlyingType = std::underlying_type_t<T>;

		constexpr EnumMask() = default;

		constexpr explicit EnumMask(std::initializer_list<T> x)
		{
			for(auto e : x)
			{
				this->set(e);
			}
		}

		[[nodiscard]] constexpr auto test(T x) const noexcept -> bool
		{
			return (this->mask & static_cast<UnderlyingType>(x)) != 0x0;
		}

		constexpr auto set(T x) noexcept -> void
		{
			this->mask |= static_cast<UnderlyingType>(x);
		}

		[[nodiscard]] constexpr auto get() const noexcept -> UnderlyingType
		{
			return this->mask;
		}

		constexpr auto remove(T x) noexcept -> void
		{
			this->mask &= ~static_cast<UnderlyingType>(x);
		}

		constexpr auto flip(T x) noexcept -> void
		{
			this->mask ^= static_cast<UnderlyingType>(x);
		}

		constexpr auto clear() noexcept -> void
		{
			this->mask = 0;
		}

	private:
		UnderlyingType mask{};
	};
}