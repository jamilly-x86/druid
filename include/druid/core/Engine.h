#pragma once

#include <druid/core/Event.h>
#include <druid/core/Object.h>
#include <druid/core/Signal.h>
#include <chrono>
#include <memory>
#include <utility>
#include <vector>

namespace druid::core
{
	class Engine;

	/// @class druid::core::Service
	/// @brief Base type for engine-managed subsystems.
	///
	/// Services receive three lifecycle calls per frame:
	/// - `update(delta)`
	/// - `update_fixed(dt_fixed)`
	/// - `update_end()`
	///
	/// Derive from `Service` to implement rendering, physics, audio, etc.
	/// A service always has access to the hosting `Engine` via `engine()`.
	class Service
	{
	public:
		/// @brief Create a service bound to an engine.
		/// @param x Engine that owns and schedules this service.
		Service(Engine& x);

		virtual ~Service() = default;

		Service(const Service&) = delete;
		Service(Service&&) noexcept = delete;
		auto operator=(const Service&) -> Service& = delete;
		auto operator=(Service&&) noexcept -> Service& = delete;

		/// @brief Variable-timestep update for per-frame work.
		/// @param x Elapsed time since the previous frame.
		virtual auto update(std::chrono::steady_clock::duration /*x*/) -> void;

		/// @brief Fixed-timestep update for deterministic simulation.
		/// @param x Fixed step duration configured on the `Engine`.
		virtual auto update_fixed(std::chrono::steady_clock::duration /*x*/) -> void;

		/// @brief End-of-frame hook for cleanup and presentation.
		virtual auto update_end() -> void;

		/// @brief Access the owning engine.
		/// @return Reference to the engine.
		[[nodiscard]] auto engine() -> Engine&;

	private:
		Engine& engine_;
	};

	template <typename T>
	concept ServiceType = std::is_base_of_v<Service, T>;

	/// @class druid::core::Engine
	/// @brief Main update/event loop and service orchestrator.
	///
	/// The Engine owns the game loop and dispatches three update phases to all
	/// registered services:
	/// - **update**: variable-timestep work each frame
	/// - **update_fixed**: fixed-timestep simulation steps (capped by a limit)
	/// - **update_end**: post-frame cleanup and rendering handoff
	///
	/// It also acts as the central event hub for window, keyboard, and mouse input,
	/// forwarding `Event` variants to subscribers.
	///
	/// @note Use `set_interval_fixed()` to control the fixed-timestep duration.
	/// @note Call `quit()` to request the loop to stop.
	///
	/// @code
	/// druid::core::Engine eng;
	/// // Configure services & callbacks...
	/// return eng.run(); // returns EXIT_SUCCESS or EXIT_FAILURE
	/// @endcode
	class Engine
	{
	public:
		static constexpr auto DefaultIntervalFixed{std::chrono::milliseconds{10}};
		static constexpr auto DefaultUpdateFixedLimit{5};

		/// @brief Construct an Engine and wire default dispatchers.
		Engine();

		/// @brief Set the interval at which the fixed update function is called.
		/// @param x The new fixed update interval.
		auto set_interval_fixed(std::chrono::steady_clock::duration x) -> void;

		/// @brief Get the current fixed update interval.
		/// @return The current fixed update interval.
		[[nodiscard]] auto get_interval_fixed() const noexcept -> std::chrono::steady_clock::duration;

		/// @brief Run the main loop of the engine. This will block until quit() is called.
		/// @return The exit code of the engine.
		auto run() -> int;

		/// @brief Quit the main loop of the engine.
		auto quit() -> void;

		/// @brief Tells whether the engine is currently running.
		/// @return True if the engine is running, false otherwise.
		[[nodiscard]] auto running() const noexcept -> bool;

		template <druid::core::ObjectType T = Object>
		[[nodiscard]] auto create_object() -> std::unique_ptr<T>
		{
			return std::make_unique<T>(*this);
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

		template <typename Callback>
		auto on_event_window(Callback&& x) -> void
		{
			on_event_window_.connect(std::forward<Callback>(x));
		}

		template <typename Callback>
		auto on_event_keyboard(Callback&& x) -> void
		{
			on_event_keyboard_.connect(std::forward<Callback>(x));
		}

		template <typename Callback>
		auto on_event_mouse(Callback&& x) -> void
		{
			on_event_mouse_.connect(std::forward<Callback>(x));
		}

		template <typename Callback>
		auto on_update(Callback&& x) -> void
		{
			on_update_.connect(std::forward<Callback>(x));
		}

		template <typename Callback>
		auto on_update_fixed(Callback&& x) -> void
		{
			on_update_fixed_.connect(std::forward<Callback>(x));
		}

		template <typename Callback>
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