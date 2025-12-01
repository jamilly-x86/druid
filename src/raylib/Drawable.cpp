#include <druid/raylib/Drawable.h>

auto druid::raylib::TriangleStrip::from_arc(druid::raylib::Arc x) -> TriangleStrip
{
	TriangleStrip strip{};
	strip.color = x.color;
	const auto angle_step = (x.angle_end - x.angle_start) / static_cast<float>(x.segments - 1);
	for (int i = 0; i < x.segments; ++i)
	{
		constexpr auto pi{3.14159265F};
		const auto angle = x.angle_start + (static_cast<float>(i) * angle_step);
		const auto rad = angle * (pi / 180.0F);
		const auto outer_x = x.x + ((x.radius + x.line_width / 2.0F) * std::cos(rad));
		const auto outer_y = x.y + ((x.radius + x.line_width / 2.0F) * std::sin(rad));
		const auto inner_x = x.x + ((x.radius - x.line_width / 2.0F) * std::cos(rad));
		const auto inner_y = x.y + ((x.radius - x.line_width / 2.0F) * std::sin(rad));
		strip.vertices.push_back({outer_x, outer_y});
		strip.vertices.push_back({inner_x, inner_y});
	}
	return strip;
}
