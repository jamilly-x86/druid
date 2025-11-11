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
}