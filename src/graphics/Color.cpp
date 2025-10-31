#include <druid/graphics/Color.h>

namespace druid::graphics
{
	const Color Color::Druid{{.r = 53, .g = 39, .b = 39}};
	const Color Color::Red{{.r = Rgba::Max}};
	const Color Color::Green{{.g = Rgba::Max}};
	const Color Color::Blue{{.b = Rgba::Max}};
	const Color Color::White{{.r = Rgba::Max, .g = Rgba::Max, .b = Rgba::Max}};
	const Color Color::Black{{}};
	const Color Color::Transparent{{.a = 0U}};
}