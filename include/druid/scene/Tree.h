#pragma once

#include <druid/scene/Node.h>
#include <flecs.h>
#include <set>
#include <unordered_map>
#include <map>

namespace druid::scene
{
	class Tree
	{
	public:
		[[nodiscard]] auto create_node() -> std::unique_ptr<Node>;
		auto make_dirty(Node::Dirty dirty, flecs::entity e) -> void;

		auto update_transforms() -> void;

	private:
		flecs::world world_;
		std::map<int, std::set<flecs::entity>> dirty_transforms_;
	};
}
