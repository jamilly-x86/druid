module;

export module druid.core.components;

export namespace druid::core
{
	struct Position
	{
		float x{};
		float y{};
	};

	struct Scale
	{
		float x{1.0F};
		float y{1.0F};
	};

	struct Size
	{
		float width{};
		float height{};
	};

	struct Transform
	{
		Position position{};
		float rotation{};
		Scale scale{};
	};
}