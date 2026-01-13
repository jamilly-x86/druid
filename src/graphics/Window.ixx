module;

#include <chrono>
#include <memory>
#include <string>
#include <string_view>

export module druid.graphics.Window;

import druid.core.Engine;
import druid.graphics.Node;
import druid.graphics.Renderer;

export namespace druid::graphics
{
	/// @class Window
	/// @brief Abstract base class for platform-agnostic window management and rendering.
	///
	/// `Window` is an engine `Service` that provides the interface for:
	/// - Window creation and management
	/// - Rendering backend (`Renderer`) lifecycle
	/// - Per-frame updates and rendering
	/// - Scene-graph root `Node` access
	///
	/// Platform-specific implementations (e.g., RaylibWindow, SDLWindow) derive from
	/// this class and provide concrete implementations for window creation, input handling,
	/// and rendering.
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
		/// @param x Owning engine instance.
		explicit Window(druid::core::Engine& x) : Service{x}
		{
		}

		/// @brief Virtual destructor for proper cleanup in derived classes.
		~Window() override = default;

		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&&) noexcept -> Window& = delete;

		/// @brief Per-frame update hook.
		///
		/// Typically performs per-frame window polling (input/events), prepares
		/// the renderer, and triggers draw traversal starting from `root_node()`.
		///
		/// @param x Time since last frame.
		auto update(std::chrono::steady_clock::duration x) -> void override = 0;

		/// @brief End-of-frame hook.
		///
		/// Typically finalizes rendering for the frame (e.g., present/swap buffers),
		/// and performs any end-of-frame housekeeping.
		auto update_end() -> void override = 0;

		/// @brief Get the root node of the window's scene graph.
		///
		/// Rendering typically traverses from this node. Users can attach child
		/// nodes to build a renderable hierarchy.
		///
		/// @return Reference to the root node.
		[[nodiscard]] auto root_node() -> Node&
		{
			return root_node_;
		}

		/// @brief Set the window title.
		/// @param x New title string.
		auto set_title(std::string_view x) -> void
		{
			title_ = x;
		}

		/// @brief Get the current window title.
		/// @return Current title string view.
		[[nodiscard]] auto get_title() -> std::string_view
		{
			return title_;
		}

		/// @brief Set the renderer for this window.
		/// @param x The renderer to use for drawing.
		auto set_renderer(std::unique_ptr<Renderer> x) -> void
		{
			renderer_ = std::move(x);
		}

		/// @brief Get the renderer for this window.
		/// @return Pointer to the renderer, or nullptr if not set.
		[[nodiscard]] auto get_renderer() -> Renderer*
		{
			return renderer_.get();
		}

	private:
		Node root_node_;
		std::unique_ptr<Renderer> renderer_;
		std::string title_{DefaultTitle};
	};
}
