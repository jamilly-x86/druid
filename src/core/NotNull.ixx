export module druid.core.NotNull;

import std;

export namespace druid::core
{
	namespace details
	{
		template <typename T, typename = void>
		struct is_comparable_to_nullptr : std::false_type
		{
		};

		template <typename T>
		struct is_comparable_to_nullptr<T, std::enable_if_t<std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value>>
			: std::true_type
		{
		};

		template <typename T>
		using value_or_reference_return_t =
			std::conditional_t<sizeof(T) < 2 * sizeof(void*) && std::is_trivially_copy_constructible<T>::value, const T, const T&>;

		[[noreturn]] inline void terminate() noexcept
		{
			std::terminate();
		}

	} // namespace details

	template <class T>
	class not_null
	{
	public:
		static_assert(details::is_comparable_to_nullptr<T>::value, "T cannot be compared to nullptr.");

		using element_type = T;

		template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
		constexpr explicit not_null(U&& u) noexcept(std::is_nothrow_move_constructible<T>::value) : ptr_(std::forward<U>(u))
		{
			if (!(ptr_ != nullptr))
				details::terminate();
		}

		template <typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
		constexpr explicit not_null(T u) noexcept(std::is_nothrow_move_constructible<T>::value) : ptr_(std::move(u))
		{
			if (!(ptr_ != nullptr))
				details::terminate();
		}

		template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
		constexpr not_null(const not_null<U>& other) noexcept(std::is_nothrow_move_constructible<T>::value) : ptr_(other.get())
		{
		}

		not_null(not_null&& other) noexcept(std::is_nothrow_copy_constructible<T>::value) = default;
		not_null(const not_null& other) = default;
		not_null& operator=(const not_null& other) = default;

		constexpr details::value_or_reference_return_t<T> get() const noexcept(noexcept(details::value_or_reference_return_t<T>(std::declval<T&>())))
		{
			return ptr_;
		}

		constexpr operator T() const
		{
			return get();
		}
		constexpr decltype(auto) operator->() const
		{
			return get();
		}
		constexpr decltype(auto) operator*() const
		{
			return *get();
		}

		not_null(std::nullptr_t) = delete;
		not_null& operator=(std::nullptr_t) = delete;

		not_null& operator++() = delete;
		not_null& operator--() = delete;
		not_null operator++(int) = delete;
		not_null operator--(int) = delete;
		not_null& operator+=(std::ptrdiff_t) = delete;
		not_null& operator-=(std::ptrdiff_t) = delete;
		void operator[](std::ptrdiff_t) const = delete;

		void swap(not_null<T>& other)
		{
			std::swap(ptr_, other.ptr_);
		}

	private:
		T ptr_;
	};

	template <typename T, std::enable_if_t<std::is_move_assignable<T>::value && std::is_move_constructible<T>::value, bool> = true>
	void swap(not_null<T>& a, not_null<T>& b)
	{
		a.swap(b);
	}

	template <class T>
	auto make_not_null(T&& t) noexcept
	{
		return not_null<std::remove_cv_t<std::remove_reference_t<T>>>{std::forward<T>(t)};
	}

	template <class T, class U>
	auto operator==(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() == rhs.get())) -> decltype(lhs.get() == rhs.get())
	{
		return lhs.get() == rhs.get();
	}

	template <class T, class U>
	auto operator!=(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(lhs.get() != rhs.get())) -> decltype(lhs.get() != rhs.get())
	{
		return lhs.get() != rhs.get();
	}

	template <class T, class U>
	auto operator<(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(std::less<>{}(lhs.get(), rhs.get())))
		-> decltype(std::less<>{}(lhs.get(), rhs.get()))
	{
		return std::less<>{}(lhs.get(), rhs.get());
	}

	template <class T, class U>
	auto operator<=(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(std::less_equal<>{}(lhs.get(), rhs.get())))
		-> decltype(std::less_equal<>{}(lhs.get(), rhs.get()))
	{
		return std::less_equal<>{}(lhs.get(), rhs.get());
	}

	template <class T, class U>
	auto operator>(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(std::greater<>{}(lhs.get(), rhs.get())))
		-> decltype(std::greater<>{}(lhs.get(), rhs.get()))
	{
		return std::greater<>{}(lhs.get(), rhs.get());
	}

	template <class T, class U>
	auto operator>=(const not_null<T>& lhs, const not_null<U>& rhs) noexcept(noexcept(std::greater_equal<>{}(lhs.get(), rhs.get())))
		-> decltype(std::greater_equal<>{}(lhs.get(), rhs.get()))
	{
		return std::greater_equal<>{}(lhs.get(), rhs.get());
	}

	template <class T, class U>
	std::ptrdiff_t operator-(const not_null<T>&, const not_null<U>&) = delete;
	template <class T>
	not_null<T> operator-(const not_null<T>&, std::ptrdiff_t) = delete;
	template <class T>
	not_null<T> operator+(const not_null<T>&, std::ptrdiff_t) = delete;
	template <class T>
	not_null<T> operator+(std::ptrdiff_t, const not_null<T>&) = delete;

	template <class T, class U = decltype(std::declval<const T&>().get()), bool = std::is_default_constructible<std::hash<U>>::value>
	struct not_null_hash
	{
		std::size_t operator()(const T& value) const
		{
			return std::hash<U>{}(value.get());
		}
	};

	template <class T, class U>
	struct not_null_hash<T, U, false>
	{
		not_null_hash() = delete;
		not_null_hash(const not_null_hash&) = delete;
		not_null_hash& operator=(const not_null_hash&) = delete;
	};

}

export namespace std
{
	template <class T>
	struct hash<druid::core::not_null<T>> : druid::core::not_null_hash<druid::core::not_null<T>>
	{
	};

}
