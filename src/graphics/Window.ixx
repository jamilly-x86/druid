module;

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <chrono>
#include <cstdint>
#include <functional>
#include <string>

export module druid.graphics.window;

import druid.core.enummask;
import druid.graphics.event;
import druid.graphics.renderer;
import druid.graphics.pipeline;

namespace druid::graphics
{
	export class Window
	{
	public:
		struct Traits
		{
			enum class Flag : uint32_t
			{
				Fullscreen = SDL_WINDOW_FULLSCREEN,
				OpenGL = SDL_WINDOW_OPENGL,
				Occluded = SDL_WINDOW_OCCLUDED,
				Hidden = SDL_WINDOW_HIDDEN,
				Borderless = SDL_WINDOW_BORDERLESS,
				Resizable = SDL_WINDOW_RESIZABLE,
				Minimized = SDL_WINDOW_MINIMIZED,
				Maximized = SDL_WINDOW_MAXIMIZED,
				Mouse_grabbed = SDL_WINDOW_MOUSE_GRABBED,
				Input_focus = SDL_WINDOW_INPUT_FOCUS,
				Mouse_focus = SDL_WINDOW_MOUSE_FOCUS,
				External = SDL_WINDOW_EXTERNAL,
				Modal = SDL_WINDOW_MODAL,
				High_pixel_density = SDL_WINDOW_HIGH_PIXEL_DENSITY,
				Mouse_capture = SDL_WINDOW_MOUSE_CAPTURE,
				Mouse_relative_mode = SDL_WINDOW_MOUSE_RELATIVE_MODE,
				Always_on_top = SDL_WINDOW_ALWAYS_ON_TOP,
				Utility = SDL_WINDOW_UTILITY,
				Tooltip = SDL_WINDOW_TOOLTIP,
				Popup_menu = SDL_WINDOW_POPUP_MENU,
				Keyboard_grabbed = SDL_WINDOW_KEYBOARD_GRABBED,
				Vulkan = SDL_WINDOW_VULKAN,
				Metal = SDL_WINDOW_METAL,
				Transparent = SDL_WINDOW_TRANSPARENT,
				Not_focusable = SDL_WINDOW_NOT_FOCUSABLE,
			};

			static constexpr int DefaultWidth{1280U};
			static constexpr int DefaultHeight{720U};

#if _WIN32 || __linux__ || __ANDROID__
			static constexpr druid::core::EnumMask<Flag> DefaultFlags{Flag::Resizable, Flag::Vulkan};
#elif __APPLE__
			static constexpr druid::core::EnumMask<Flag> DefaultFlags{Flag::Resizable, Flag::Metal};
#elif __EMSCRIPTEN__
			static constexpr druid::core::EnumMask<Flag> DefaultFlags{Flag::Resizable, Flag::OpenGL};
#endif

			std::string title;
			int width{Traits::DefaultWidth};
			int height{Traits::DefaultHeight};
			druid::core::EnumMask<Flag> flags{DefaultFlags};
		};

		Window() : initialized{SDL_Init(SDL_INIT_VIDEO)}
		{
			this->initialize(this->traits);
		}

		explicit Window(Traits x) : traits{std::move(x)}, initialized{SDL_Init(SDL_INIT_VIDEO)}
		{
			this->initialize(this->traits);
		}

		~Window()
		{
			this->close();
			SDL_Quit();
		}

		Window(const Window&) = delete;
		auto operator=(const Window&) -> Window& = delete;

		Window(Window&&) noexcept = default;
		auto operator=(Window&&) noexcept -> Window& = delete;

		[[nodiscard]] auto getRenderer() const -> Renderer*
		{
			return this->renderer.get();
		}

		[[nodiscard]] auto getDefaultPipeline() const -> Pipeline*
		{
			return this->defaultPipeline.get();
		}

		[[nodiscard]] auto open() const -> bool
		{
			return this->initialized;
		}

		auto close() -> void
		{
			this->initialized = false;
			SDL_DestroyWindow(this->window);
			this->window = nullptr;
		}

		[[nodiscard]] auto handle() const -> SDL_Window*
		{
			return this->window;
		}

		[[nodiscard]] auto width() const noexcept -> uint32_t
		{
			return this->traits.width;
		}

		[[nodiscard]] auto height() const noexcept -> uint32_t
		{
			return this->traits.height;
		}

		auto processEvents() const noexcept -> void
		{
			SDL_Event e{};
			while(SDL_PollEvent(&e) == true)
			{
				if(e.type >= SDL_EventType::SDL_EVENT_WINDOW_FIRST and e.type <= SDL_EventType::SDL_EVENT_WINDOW_LAST)
				{
					EventWindow event{};
					event.timestamp = std::chrono::steady_clock::now();
					event.type = static_cast<EventWindow::Type>(e.type);

					SDL_GetWindowSize(this->window, &event.width, &event.height);
					SDL_GetWindowPosition(this->window, &event.x, &event.y);

					if(this->onEventWindow)
					{
						this->onEventWindow(event);
					}
				}
			}
		}

		auto onEvent(std::function<void(EventWindow)> x) noexcept -> void
		{
			this->onEventWindow = std::move(x);
		}

	private:
		auto initialize(const Traits& x) -> void
		{
			this->window = SDL_CreateWindow(x.title.c_str(), x.width, x.height, static_cast<unsigned long long>(x.flags.get()));
			this->initialized &= (this->window != nullptr);

			if(this->initialized == false)
			{
				return;
			}

			this->renderer = std::make_unique<Renderer>(*this->window);
			this->initialized &= this->renderer->handle() != nullptr;

			if(this->initialized == false)
			{
				return;
			}

			this->defaultPipeline = this->renderer->createPipeline();
		}

		SDL_Window* window{};
		std::unique_ptr<Renderer> renderer;
		std::unique_ptr<Pipeline> defaultPipeline;
		std::function<void(EventWindow)> onEventWindow;
		Traits traits{};
		bool initialized{};
	};
}