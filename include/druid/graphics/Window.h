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
	/// @class Window
	/// @brief Graphics service responsible for window management and rendering.
	///
	/// `Window` is an engine `Service` that typically:
	/// - Creates and owns the graphics window and rendering backend (`Renderer`).
	/// - Drives per-frame rendering during `update()` and end-of-frame presentation
	///   during `update_end()`.
	/// - Provides a root scene-graph `Node` as the entry point for rendering content.
	///
	/// The exact backend implementation (e.g., Raylib/rlgl) is encapsulated by
	/// `Renderer` and configured internally by this service.
	class Window : public druid::core::Service
	{
	public:
		/// @brief Default window title.
		static constexpr auto DefaultTitle{"Druid"};
		/// @brief Default window width in pixels.
		static constexpr auto DefaultWidth{1280};
		/// @brief Default window height in pixels.
		static constexpr auto DefaultHeight{720};

		/// @brief Construct the window service and bind it to the engine.
		///
		/// Implementations typically allocate the renderer, open a native window,
		/// and initialize any graphics state required for drawing.
		///
		/// @param x Owning engine instance.
		explicit Window(druid::core::Engine& x);

		/// @brief Destroy the window service.
		///
		/// Implementations should release renderer resources and close the window.
		~Window() override;

		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&&) noexcept -> Window& = delete;

		/// @brief Per-frame update hook.
		///
		/// Typically performs per-frame window polling (input/events), prepares
		/// the renderer, and triggers draw traversal starting from `root_node()`.
		///
		/// @param x Frame delta time since the previous update.
		auto update(std::chrono::steady_clock::duration x) -> void override;

		/// @brief End-of-frame hook.
		///
		/// Typically finalizes rendering for the frame (e.g., present/swap buffers),
		/// and performs any end-of-frame housekeeping.
		auto update_end() -> void override;

		/// @brief Get the root node of the window's scene graph.
		///
		/// Rendering typically traverses from this node. Users can attach child
		/// nodes to build a renderable hierarchy.
		///
		/// @return Reference to the root node.
		[[nodiscard]] auto root_node() -> Node&;

		/// @brief Set the window title.
		/// @param x New title string.
		auto set_title(std::string_view x) -> void;

		/// @brief Get the current window title.
		/// @return Current title string view.
		[[nodiscard]] auto get_title() -> std::string_view;

	private:
		Node root_node_;
		std::unique_ptr<Renderer> renderer_;
		std::string title_{DefaultTitle};
		int width_{DefaultWidth};
		int height_{DefaultHeight};
	};
}
