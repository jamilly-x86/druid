#include <druid/graphics/Window.h>
#include <cstdlib>

namespace druid::graphics
{
	Window::Window(druid::core::Engine& x) : Service{x}
	{
		InitWindow(width_, height_, title_.c_str());
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		renderer_ = std::make_unique<renderer::Raylib>();
	}

	Window::~Window()
	{
		CloseWindow();
	}

	auto Window::update(std::chrono::steady_clock::duration /*x*/) -> void
	{
		// Handle window events and input
		if (WindowShouldClose())
		{
			engine().quit();
		}
	}

	auto Window::update_end() -> void
	{
		// Render the scene
		if (renderer_)
		{
			renderer_->begin(Color::Black);
			root_node_.draw(*renderer_);
			renderer_->end();
		}
	}

	auto Window::set_title(std::string_view x) -> void
	{
		title_ = x;
		SetWindowTitle(title_.c_str());
	}

	auto Window::get_title() -> std::string_view
	{
		return title_;
	}

	auto Window::convert_raylib_key(KeyboardKey key) -> druid::core::EventKeyboard::Key
	{
		// Basic key conversion - can be expanded
		switch (key)
		{
		case KEY_SPACE: return druid::core::EventKeyboard::Key::Space;
		case KEY_ESCAPE: return druid::core::EventKeyboard::Key::Escape;
		case KEY_ENTER: return druid::core::EventKeyboard::Key::Enter;
		default: return druid::core::EventKeyboard::Key::None;
		}
	}
}