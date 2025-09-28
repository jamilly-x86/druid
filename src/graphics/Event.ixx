module;

#include <SDL3/SDL_events.h>
#include <chrono>
#include <variant>

export module druid.graphics.event;

namespace druid::graphics
{
	export struct EventWindow
	{
		enum class Type : std::uint16_t
		{
			None = SDL_EVENT_FIRST,
			Shown = SDL_EVENT_WINDOW_SHOWN,
			Hidden = SDL_EVENT_WINDOW_HIDDEN,
			Exposed = SDL_EVENT_WINDOW_EXPOSED,
			Moved = SDL_EVENT_WINDOW_MOVED,
			Resized = SDL_EVENT_WINDOW_RESIZED,
			Pixel_size_changed = SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
			Metal_view_resized = SDL_EVENT_WINDOW_METAL_VIEW_RESIZED,
			Minimized = SDL_EVENT_WINDOW_MINIMIZED,
			Maximized = SDL_EVENT_WINDOW_MAXIMIZED,
			Restored = SDL_EVENT_WINDOW_RESTORED,
			Mouse_enter = SDL_EVENT_WINDOW_MOUSE_ENTER,
			Mouse_leave = SDL_EVENT_WINDOW_MOUSE_LEAVE,
			Focus_gained = SDL_EVENT_WINDOW_FOCUS_GAINED,
			Focus_lost = SDL_EVENT_WINDOW_FOCUS_LOST,
			Close_requested = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
			Hit_test = SDL_EVENT_WINDOW_HIT_TEST,
			Iccprof_changed = SDL_EVENT_WINDOW_ICCPROF_CHANGED,
			Display_changed = SDL_EVENT_WINDOW_DISPLAY_CHANGED,
			Display_scale_changed = SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED,
			Safe_area_changed = SDL_EVENT_WINDOW_SAFE_AREA_CHANGED,
			Occluded = SDL_EVENT_WINDOW_OCCLUDED,
			Enter_fullscreen = SDL_EVENT_WINDOW_ENTER_FULLSCREEN,
			Leave_fullscreen = SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,
			Destroyed = SDL_EVENT_WINDOW_DESTROYED,
			Hdr_state_changed = SDL_EVENT_WINDOW_HDR_STATE_CHANGED,
			First = SDL_EVENT_WINDOW_FIRST
		};

		std::chrono::steady_clock::time_point timestamp;
		int x{};
		int y{};
		int width{};
		int height{};
		Type type{Type::None};
	};

	using Event = std::variant<std::monostate, EventWindow>;
}
