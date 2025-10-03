module;

#include <chrono>

export module druid.core.engine;

import druid.core.object;

namespace druid::core
{
	export class Engine : public druid::core::Object
	{
	public:
		static constexpr auto DefaultIntervalFixed{std::chrono::milliseconds{10}};
		static constexpr auto DefaultUpdateFixedLimit{5};

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

					auto count = 0;

					// This implements a maximum
					while (accumulate_ >= interval_fixed_ && count < update_fixed_limit_)
					{
						accumulate_ -= interval_fixed_;
						count++;

						update_fixed(interval_fixed_);
					}

					// Update logic here.
					update(delta);
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

	private:
		std::chrono::steady_clock::time_point start_;
		std::chrono::steady_clock::duration accumulate_{};
		std::chrono::steady_clock::duration interval_fixed_{DefaultIntervalFixed};
		int update_fixed_limit_{DefaultUpdateFixedLimit};
		bool running_{false};
	};
}