#pragma once

#include <exception>
#include <functional>
#include <typeinfo>

namespace druid::core
{
	/// @brief Primary template declaration for Signal.
	/// @tparam T Function-signature type (e.g. R(Args...)).
	/// @note This primary template is specialized for function types below.
	template <typename T>
	class Signal;

	/// @brief Specialization of Signal for function types R(Args...).
	/// @tparam R The return type of the signal invocation.
	/// @tparam Args The parameter types accepted by the signal.
	/// @details
	/// This is a thin wrapper around sigslot::signal<Args...>. It forwards
	/// connect and invocation operations to the underlying sigslot implementation
	/// while preserving a function-signature based interface.
	///
	/// The primary purpose of this wrapper is to ensure that the necessary
	/// RTTI/typeinfo for the function signature is emitted from translation units
	/// that include this header. Some toolchains (notably GCC at the time of
	/// writing) do not yet fully support exporting headers as modules; emitting
	/// typeinfo from the header helps avoid missing-symbol or RTTI-related issues
	/// when consumers link against compiled artifacts.
	template <typename R, typename... Args>
	class Signal<R(Args...)>
	{
	public:
		template <typename Callback>
		auto connect(Callback&& callback) -> void
		{
			signal_ = std::forward<Callback>(callback);
		}

		auto operator()(Args... args) const noexcept -> void
		{
			if (!signal_)
			{
				return;
			}

			try
			{
				signal_(std::forward<Args>(args)...);
			}
			catch (const std::exception& e)
			{
				(void)e;
			}
		}

	private:
		std::function<R(Args...)> signal_{};
	};
}