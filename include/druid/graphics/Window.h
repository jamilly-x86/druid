#pragma once

#include <druid/core/Engine.h>
#include <druid/core/Event.h>
#include <druid/graphics/Color.h>
#include <druid/graphics/Node.h>
#include <druid/graphics/Renderer.h>
#include <chrono>
#include <memory>
#include <string>

namespace druid::graphics
{
	class Window : public druid::core::Service
	{
	public:
		static constexpr auto DefaultTitle{"Druid"};
		static constexpr auto DefaultWidth{1280};
		static constexpr auto DefaultHeight{720};

		Window(druid::core::Engine& x);
		~Window() override;

		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&&) noexcept -> Window& = delete;

		auto update(std::chrono::steady_clock::duration x) -> void override;
		auto update_end() -> void override;

		[[nodiscard]] auto root_node() -> Node&;

		auto set_title(std::string_view x) -> void;
		[[nodiscard]] auto get_title() -> std::string_view;

	private:
		Node root_node_;
		std::unique_ptr<Renderer> renderer_;
		std::string title_{DefaultTitle};
		int width_{DefaultWidth};
		int height_{DefaultHeight};
	};
}