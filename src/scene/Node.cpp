#include <druid/scene/Heirarchy.h>
#include <druid/scene/Node.h>
#include <druid/scene/Transform.h>
#include <druid/scene/Tree.h>

namespace druid::scene
{
	Node::Node(Tree& x, flecs::entity e) : tree_{&x}, entity_{e}
	{
		entity_.set<Heirarchy>({.depth = 0});
	}

	Node::~Node()
	{
		if (entity_.is_alive())
		{
			entity_.destruct();
		}
	}

	auto Node::set_transform(const Transform& x) -> void
	{
		// Allocate Transform if it doesn't exist.
		entity_.add<Transform>();
		auto& t = entity_.get_mut<Transform>();
		t = x;

		make_dirty(Dirty::Transform);
	}

	auto Node::get_transform() const -> const Transform&
	{
		// Allocate Transform if it doesn't exist.
		entity_.add<Transform>();
		return entity_.get<Transform>();
	}

	auto Node::clear_transform() const -> void
	{
		// Remove simply returns the entity so ignore the return value warning.
		(void)entity_.remove<Transform>();
	}

	auto Node::add_child(Node* x) -> void
	{
		x->entity_.child_of(entity_);
		x->entity_.set<Heirarchy>({.depth = entity_.get<Heirarchy>().depth + 1});
		x->make_dirty(Dirty::NodeAdded);
	}

	auto Node::remove_child(Node* x) -> void
	{
		x->entity_.remove(flecs::ChildOf, entity_);
		x->make_dirty(Dirty::NodeRemoved);
	}

	auto Node::make_dirty(Dirty dirty) -> void
	{
		if (dirty == Dirty::Transform)
		{
			entity_.add<Transform>();
		}

		tree_->make_dirty(dirty, entity_);
	}
}