module;

#include <sigslot/signal.hpp>

export module druid.core.signal;

export namespace druid::core
{
	template <typename... Args>
	class Signal
	{
	public:
		auto connect(auto callback) -> void
		{
			signal_.connect(std::forward<decltype(callback)>(callback));
		}

		auto operator()(Args... args) const -> void
		{
			signal_(std::forward<Args>(args)...);
		}

	private:
		sigslot::signal<Args...> signal_{};
	};
}