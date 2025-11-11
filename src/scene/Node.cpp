#include <druid/scene/Node.h>
#include <druid/scene/Transform.h>
#include <druid/scene/Tree.h>

namespace druid::scene
{
	Node::Node(Tree& x, flecs::entity e) : tree_{&x}, entity_{e}
	{
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

	auto Node::make_dirty(Dirty dirty) -> void
	{
		tree_->make_dirty(dirty, entity_);
	}
}