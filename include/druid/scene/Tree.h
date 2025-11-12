#pragma once

#include <druid/scene/Node.h>
#include <flecs.h>
#include <unordered_map>
#include <set>

namespace druid::scene
{
	class Tree
	{
	public:
		[[nodiscard]] auto create_node() -> std::unique_ptr<Node>;
		auto make_dirty(Node::Dirty dirty, flecs::entity e) -> void;

		auto update_transforms() const -> void;
		
	private:
		flecs::world world_;
		std::unordered_map<Node::Dirty, std::set<flecs::entity>> dirty_;
	};
}
