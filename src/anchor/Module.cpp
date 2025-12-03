#include <druid/anchor/Module.h>

#include <druid/anchor/Components.h>

using druid::anchor::Anchor;
using druid::anchor::Module;

Module::Module(flecs::world& x)
{
	x.module<Module>();

	// Ensure the necessary components are present when Anchor is added.
	x.component<Anchor>();

	x.system<Anchor>()
		// I need to ensure these calculations occur after Transform and Size have been modified (e.g., by a layout system).
		.kind(flecs::PostUpdate)
		.each(
			[](flecs::entity e, Anchor& anchor)
			{
				const auto target = anchor.target.entity;
				const auto target_point = anchor.target.point;

				druid::core::Position target_position{};

				const auto* target_transform = target.try_get<druid::core::Transform>();

				if (target_transform != nullptr)
				{
					target_position = target_transform->position;
				}

				const auto* target_size = target.try_get<druid::core::Size>();

				if (target_size == nullptr)
				{
					return;
				}

				switch (target_point)
				{
					case Anchor::Point::Center:
					{
						const auto center_x = target_position.x + (target_size->width / 2.0F);
						const auto center_y = target_position.y + (target_size->height / 2.0F);
						e.set<druid::core::Transform>({.position = {.x = center_x, .y = center_y}});
					}
					break;

					default:
						break;
				}
			});
}
