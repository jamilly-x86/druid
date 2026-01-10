module;

#include <cstdlib>

export module druid.core.Engine;

import std;
import druid.core.Event;
import druid.core.Object;
import druid.core.Signal;

namespace druid::core
{
	export class Engine;

	/// @class Service
	/// @brief Base class for engine services that participate in the main update loop.
	///
	/// A `Service` is owned by an `Engine` and may receive per-frame updates, fixed-timestep
	/// updates, and an end-of-frame callback. Subclasses override the virtual update methods
	/// to implement behavior.
	///
	/// @note Services are non-copyable and non-movable because they store a reference to the
	///       owning engine.
	export class Service
	{
	public:
		/// @brief Construct a service bound to the given engine.
		/// @param x Owning engine instance.
		Service(Engine& x) : engine_{x}
		{
		}

		/// @brief Virtual destructor for polymorphic deletion.
		virtual ~Service() = default;

		Service(const Service&) = delete;
		Service(Service&&) noexcept = delete;
		auto operator=(const Service&) -> Service& = delete;
		auto operator=(Service&&) noexcept -> Service& = delete;

		/// @brief Per-frame update hook.
		///
		/// Called once per engine frame with the frame delta time.
		/// Default implementation does nothing.
		virtual auto update(std::chrono::steady_clock::duration /*x*/) -> void
		{
		}

		/// @brief Fixed-timestep update hook.
		///
		/// Called zero or more times per frame depending on the accumulated time and the
		/// fixed update interval configured on the engine. Default implementation does nothing.
		virtual auto update_fixed(std::chrono::steady_clock::duration /*x*/) -> void
		{
		}

		/// @brief End-of-frame hook.
		///
		/// Called once at the end of each frame after all update and fixed update work for
		/// that frame is complete. Default implementation does nothing.
		virtual auto update_end() -> void
		{
		}

		/// @brief Access the owning engine.
		/// @return Reference to the engine that owns this service.
		[[nodiscard]] auto engine() -> Engine&
		{
			return engine_;
		}

	private:
		Engine& engine_;
	};

	/// @concept ServiceType
	/// @brief Constrains types to those derived from druid::core::Service.
	export template <typename T>
	concept ServiceType = std::is_base_of_v<Service, T>;

	/// @class Engine
	/// @brief Core runtime that manages services, update cadence, and event dispatch.
	///
	/// The engine provides:
	/// - A blocking `run()` loop that continues until `quit()` is called.
	/// - Per-frame updates (`update`) and fixed-timestep updates (`update_fixed`).
	/// - End-of-frame callbacks (`update_end`).
	/// - Event dispatch to specialized channels (window, keyboard, mouse).
	///
	/// Users can either:
	/// - Create subclassed `Service` objects via `create_service<T>()`, or
	/// - Subscribe callbacks to the signal channels (on_update, on_update_fixed, etc.)
	export class Engine
	{
	public:
		/// @brief Default fixed update interval (10ms).
		static constexpr auto DefaultIntervalFixed{std::chrono::milliseconds{10}};
		/// @brief Default maximum number of fixed updates allowed per frame.
		///
		/// This limit helps prevent spiraling fixed-update catchup in extreme stalls.
		static constexpr auto DefaultUpdateFixedLimit{5};

		/// @brief Construct an engine with default timing configuration.
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

		/// @brief Create an engine-owned object.
		///
		/// The default template argument creates a base `Object`. The object is returned
		/// as a `std::unique_ptr` to allow the caller to own and manage its lifetime.
		///
		/// @tparam T Object type (must satisfy druid::core::ObjectType).
		/// @return Newly created object instance.
		template <druid::core::ObjectType T = Object>
		[[nodiscard]] auto create_object() -> std::unique_ptr<T>
		{
			return std::make_unique<T>(*this);
		}

		/// @brief Create and register a service owned by the engine.
		///
		/// The created service is stored internally and will be updated during the
		/// engine loop. A reference is returned for convenience.
		///
		/// @tparam T Service type (must derive from Service).
		/// @return Reference to the created service.
		template <ServiceType T>
		[[nodiscard]] auto create_service() -> T&
		{
			auto service = std::make_unique<T>(*this);
			auto* ptr = service.get();
			services_.emplace_back(std::move(service));
			return *ptr;
		}

		/// @brief Utility helper for composing multiple visitors into one (std::visit).
		///
		/// Example:
		/// @code
		/// std::visit(Engine::Overloaded{
		///   [](const EventWindow&){},
		///   [](const EventKeyboard&){},
		/// }, event_variant);
		/// @endcode
		template <typename... Ts>
		struct Overloaded : Ts...
		{
			using Ts::operator()...;
		};

		/// @brief Dispatch an event to the appropriate event signal channel.
		///
		/// Implementations typically route based on the concrete event type
		/// (e.g., window vs keyboard vs mouse).
		///
		/// @param x Event instance to dispatch.
		auto event(const Event& x) -> void
		{
			std::visit(
				Overloaded{
					[this](const EventWindow& x) { on_event_window_(x); },
					[this](const EventKeyboard& x) { on_event_keyboard_(x); },
					[this](const EventMouse& x) { on_event_mouse_(x); },
					[](auto) {},
				},
				x);
		}

		/// @brief Subscribe a callback to window events.
		/// @tparam Callback Callable type with signature `void(const EventWindow&)`.
		/// @param x Callback to connect.
		template <typename Callback>
		auto on_event_window(Callback&& x) -> void
		{
			on_event_window_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe a callback to keyboard events.
		/// @tparam Callback Callable type with signature `void(const EventKeyboard&)`.
		/// @param x Callback to connect.
		template <typename Callback>
		auto on_event_keyboard(Callback&& x) -> void
		{
			on_event_keyboard_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe a callback to mouse events.
		/// @tparam Callback Callable type with signature `void(const EventMouse&)`.
		/// @param x Callback to connect.
		template <typename Callback>
		auto on_event_mouse(Callback&& x) -> void
		{
			on_event_mouse_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe a callback to per-frame updates.
		/// @tparam Callback Callable type with signature `void(std::chrono::steady_clock::duration)`.
		/// @param x Callback to connect.
		template <typename Callback>
		auto on_update(Callback&& x) -> void
		{
			on_update_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe a callback to fixed-timestep updates.
		/// @tparam Callback Callable type with signature `void(std::chrono::steady_clock::duration)`.
		/// @param x Callback to connect.
		template <typename Callback>
		auto on_update_fixed(Callback&& x) -> void
		{
			on_update_fixed_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe a callback to end-of-frame callbacks.
		/// @tparam Callback Callable type with signature `void()`.
		/// @param x Callback to connect.
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
