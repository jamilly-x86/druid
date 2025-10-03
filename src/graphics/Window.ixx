module;

#include <raylib.h>
#include <string>
#include <string_view>

// Don't remove. Required to fix gcc compiler error.
#include <typeinfo>

export module druid.graphics.window;
import druid.core.object;
import druid.core.engine;
import druid.core.event;
import druid.graphics.node;

namespace druid::graphics
{
	export class Window : public druid::core::Object
	{
	public:
		static constexpr auto DefaultTitle{"Druid"};
		static constexpr auto DefaultWidth{1280};
		static constexpr auto DefaultHeight{720};

		Window()
		{
			SetWindowState(FLAG_WINDOW_RESIZABLE);
			InitWindow(width_, height_, title_.c_str());

			on_added([this](auto* parent) { engine_ = dynamic_cast<core::Engine*>(parent); });

			on_update(
				[this](auto)
				{
					if (!engine_)
					{
						return;
					}

					if (WindowShouldClose())
					{
						engine_->event(core::Event::Type::Close);
					}
				});

			on_update_end(
				[this]
				{
					ClearBackground(BLACK);
					BeginDrawing();

					root_node_.draw();
					DrawFPS(0, 0);

					EndDrawing();
				});
		}

		~Window() override
		{
			CloseWindow();
		}

		Window(const Window&) = delete;
		Window(Window&&) noexcept = delete;
		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&&) noexcept -> Window& = delete;

		[[nodiscard]] auto root_node() -> Node&
		{
			return root_node_;
		}

		auto set_title(std::string_view x) -> void
		{
			title_ = x;
			SetWindowTitle(title_.c_str());
		}

		[[nodiscard]] auto get_title() -> std::string_view
		{
			return title_;
		}

		auto resize(int w, int h) -> void
		{
			width_ = w;
			height_ = h;
			SetWindowSize(w, h);
		}

		auto move(int x, int y) -> void
		{
			x_ = x;
			y_ = y;
			SetWindowPosition(x, y);
		}

		[[nodiscard]] auto x() const -> int
		{
			return x_;
		}

		[[nodiscard]] auto y() const -> int
		{
			return y_;
		}

		[[nodiscard]] auto width() const -> int
		{
			return width_;
		}

		[[nodiscard]] auto height() const -> int
		{
			return height_;
		}

	private:
		std::string title_{DefaultTitle};
		int x_{};
		int y_{};
		int width_{DefaultWidth};
		int height_{DefaultHeight};
		Node root_node_;
		core::Engine* engine_{};
	};
}