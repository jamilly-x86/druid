#pragma once

#include <chrono>
#include <memory>
#include <utility>
#include <variant>
#include <vector>
#include <druid/core/Event.h>
#include <druid/core/Signal.h>

namespace druid::core
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

		Engine();

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
		auto run() -> int;

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

		template <typename... Ts>
		struct Overloaded : Ts...
		{
			using Ts::operator()...;
		};

		auto event(const Event& x) -> void;

		template<typename Callback>
		auto on_event_window(Callback&& x) -> void
		{
			on_event_window_.connect(std::forward<Callback>(x));
		}

		template<typename Callback>
		auto on_event_keyboard(Callback&& x) -> void
		{
			on_event_keyboard_.connect(std::forward<Callback>(x));
		}

		template<typename Callback>
		auto on_event_mouse(Callback&& x) -> void
		{
			on_event_mouse_.connect(std::forward<Callback>(x));
		}

		template<typename Callback>
		auto on_update(Callback&& x) -> void
		{
			on_update_.connect(std::forward<Callback>(x));
		}

		template<typename Callback>
		auto on_update_fixed(Callback&& x) -> void
		{
			on_update_fixed_.connect(std::forward<Callback>(x));
		}

		template<typename Callback>
		auto on_update_end(Callback&& x) -> void
		{
			on_update_end_.connect(std::forward<Callback>(x));
		}

	private:
		std::vector<std::unique_ptr<Service>> services_;

		Signal<void(std::chrono::steady_clock::duration)> on_update_;
		Signal<void(std::chrono::steady_clock::duration)> on_update_fixed_;
		Signal<void()> on_update_end_;
		Signal<void(const druid::core::EventWindow&)> on_event_window_;
		Signal<void(const druid::core::EventKeyboard&)> on_event_keyboard_;
		Signal<void(const druid::core::EventMouse&)> on_event_mouse_;

		std::chrono::steady_clock::time_point start_;
		std::chrono::steady_clock::duration accumulate_{};
		std::chrono::steady_clock::duration interval_fixed_{DefaultIntervalFixed};
		int update_fixed_limit_{DefaultUpdateFixedLimit};
		bool running_{false};
	};
}