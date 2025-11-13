#include <druid/scene/Heirarchy.h>
#include <druid/scene/Tree.h>

namespace druid::scene
{
	auto Tree::create_node() -> std::unique_ptr<Node>
	{
		return std::make_unique<Node>(*this, world_.entity());
	}

	auto Tree::make_dirty(Node::Dirty dirty, flecs::entity e) -> void
	{
		const auto depth = e.get<Heirarchy>().depth;
		dirty_transforms_[depth].insert(e);
		e.children([this, dirty](flecs::entity child) { make_dirty(dirty, child); });
	}

	auto Tree::update_transforms() -> void
	{
		for (const auto& [depth, entities] : dirty_transforms_)
		{
			// Nodes at this depth have no parent. Therefore, their local transform
			// is their global transform.
			if (depth == 0)
			{
				for (auto entity : entities)
				{
					auto& tx = entity.get_mut<Transform>();
					tx.global = tx.local;
				}

				continue;
			}

			for (auto entity : entities)
			{
				auto parent = entity.parent();
				const auto& tx_parent = parent.get<Transform>();

				auto& tx = entity.get_mut<Transform>();
				tx.global = tx_parent.global * tx.local;
			}
		}

		dirty_transforms_.clear();
	}
}