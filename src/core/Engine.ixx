module;

#include <chrono>
#include <vector>

export module druid.core.engine;

import druid.core.event;
import druid.core.signal;

export namespace druid::core
{
	class Engine;
	class Service
	{
	public:
		Service(Engine& x) : engine_{x}
		{
		}

		virtual ~Service() = default;

		Service(const Service&) = delete;
		Service(Service&&) noexcept = delete;
		auto operator=(const Service&) -> Service& = delete;
		auto operator=(Service&&) noexcept -> Service& = delete;

		virtual auto update(std::chrono::steady_clock::duration /*x*/) -> void
		{
		}

		virtual auto update_fixed(std::chrono::steady_clock::duration /*x*/) -> void
		{
		}

		virtual auto update_end() -> void
		{
		}

		[[nodiscard]] auto engine() -> Engine&
		{
			return engine_;
		}

	private:
		Engine& engine_;
	};
	template <typename T>
	concept ServiceType = std::is_base_of_v<Service, T>;
	class Engine
	{
	public:
		static constexpr auto DefaultIntervalFixed{std::chrono::milliseconds{10}};
		static constexpr auto DefaultUpdateFixedLimit{5};

		Engine()
		{
			on_update(
				[this](auto x)
				{
					for (auto& service : services_)
					{
						service->update(x);
					}
				});

			on_update_fixed(
				[this](auto x)
				{
					for (auto& service : services_)
					{
						service->update_fixed(x);
					}
				});

			on_update_end(
				[this]
				{
					for (auto& service : services_)
					{
						service->update_end();
					}
				});
		}

		/// @brief Set the interval at which the fixed update function is called.
		/// @param x The new fixed update interval.
		auto set_interval_fixed(std::chrono::steady_clock::duration x) -> void
		{
			interval_fixed_ = x;
		}

		/// @brief Get the current fixed update interval.
		/// @return The current fixed update interval.
		[[nodiscard]] auto get_interval_fixed() const noexcept -> std::chrono::steady_clock::duration
		{
			return interval_fixed_;
		}

		/// @brief Run the main loop of the engine. This will block until quit() is called.
		/// @return The exit code of the engine.
		auto run() -> int
		{
			try
			{
				running_ = true;

				start_ = std::chrono::steady_clock::now();

				while (running_)
				{
					const auto now = std::chrono::steady_clock::now();
					const auto delta = now - start_;
					start_ = now;
					accumulate_ += delta;

					on_update_(delta);

					auto count = 0;

					while (accumulate_ >= interval_fixed_ && count < update_fixed_limit_)
					{
						accumulate_ -= interval_fixed_;
						count++;

						on_update_fixed_(interval_fixed_);
					}

					on_update_end_();
				}

				return EXIT_SUCCESS;
			}
			catch (...)
			{
				return EXIT_FAILURE;
			}
		}

		/// @brief Quit the main loop of the engine.
		auto quit() -> void
		{
			running_ = false;
		}

		/// @brief Tells whether the engine is currently running.
		/// @return True if the engine is running, false otherwise.
		[[nodiscard]] auto running() const noexcept -> bool
		{
			return running_;
		}

		template <ServiceType T>
		[[nodiscard]] auto create_service() -> T&
		{
			auto service = std::make_unique<T>(*this);
			auto* ptr = service.get();
			services_.emplace_back(std::move(service));
			return *ptr;
		}

		auto event(Event::Type x) -> void
		{
			switch (x)
			{
				case Event::Type::Close:
					quit();
					break;
				default:
					break;
			}
		}

		auto on_update(auto x) -> void
		{
			on_update_.connect(std::forward<decltype(x)>(x));
		}

		auto on_update_fixed(auto x) -> void
		{
			on_update_fixed_.connect(std::forward<decltype(x)>(x));
		}

		auto on_update_end(auto x) -> void
		{
			on_update_end_.connect(std::forward<decltype(x)>(x));
		}

	private:
		std::vector<std::unique_ptr<Service>> services_;

		Signal<std::chrono::steady_clock::duration> on_update_;
		Signal<std::chrono::steady_clock::duration> on_update_fixed_;
		Signal<> on_update_end_;

		std::chrono::steady_clock::time_point start_;
		std::chrono::steady_clock::duration accumulate_{};
		std::chrono::steady_clock::duration interval_fixed_{DefaultIntervalFixed};
		int update_fixed_limit_{DefaultUpdateFixedLimit};
		bool running_{false};
	};
}