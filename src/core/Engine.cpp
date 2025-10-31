#include <druid/core/Engine.h>
#include <cstdlib>

namespace druid::core
{
	// This feels so wrong.
	// NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
	Engine* singleton{};

	auto Engine::instance() -> Engine*
	{
		return singleton;
	}

	Engine::Engine()
	{
		singleton = this;

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

	auto Engine::run() -> int
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

	auto Engine::event(const Event& x) -> void
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

	Service::Service(Engine& x) : engine_{x}
	{
	}

	auto Service::update(std::chrono::steady_clock::duration /*x*/) -> void
	{
	}

	auto Service::update_fixed(std::chrono::steady_clock::duration /*x*/) -> void
	{
	}

	auto Service::update_end() -> void
	{
	}

	auto Service::engine() -> Engine&
	{
		return engine_;
	}

	auto Engine::set_interval_fixed(std::chrono::steady_clock::duration x) -> void
	{
		interval_fixed_ = x;
	}

	auto Engine::get_interval_fixed() const noexcept -> std::chrono::steady_clock::duration
	{
		return interval_fixed_;
	}

	auto Engine::quit() -> void
	{
		running_ = false;
	}

	auto Engine::running() const noexcept -> bool
	{
		return running_;
	}
}