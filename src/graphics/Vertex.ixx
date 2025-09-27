export module druid.graphics.vertex;

import druid.graphics.color;

using druid::graphics::RgbaF;

export namespace druid::graphics
{
	struct Vec2i
	{
		int x{};
		int y{};
	};

	struct Vec2F
	{
		float x{};
		float y{};
	};

	struct Vertex
	{
		Vec2F position{};
		RgbaF color{};
		Vec2F texCoords{};
	};
}