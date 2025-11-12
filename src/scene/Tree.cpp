#include <druid/scene/Tree.h>

namespace druid::scene
{
	auto Tree::create_node() -> std::unique_ptr<Node>
	{
		return std::make_unique<Node>(*this, world_.entity());
	}

	auto Tree::make_dirty(Node::Dirty dirty, flecs::entity e) -> void
	{
		dirty_[dirty].insert(e);
	}

	auto Tree::update_transforms() const -> void
	{
		const auto it = dirty_.find(Node::Dirty::Transform);

		if (it == std::end(dirty_))
		{
			return;
		}

		for (auto entity : it->second)
		{
			if (!entity.has<Transform>())
			{
				continue;
			}

			const auto& parent_transform = entity.get<Transform>();

			entity.children(
				[parent_transform](flecs::entity child)
				{
					if (!child.has<Transform>())
					{
						return;
					}

					auto& child_transform = child.get_mut<Transform>();
					child_transform.global = parent_transform.global * child_transform.local;
				});
		}
	}
}