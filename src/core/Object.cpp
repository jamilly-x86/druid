#include <druid/core/Object.h>
#include <algorithm>

namespace druid::core
{
	Object::Object(Engine& x) : engine_{&x}
	{
	}

	Object::~Object() noexcept
	{
		on_destroyed_();
	}

	auto Object::set_name(std::string_view x) -> void
	{
		this->name_ = x;
	}

	auto Object::get_name() const noexcept -> std::string_view
	{
		return this->name_;
	}

	auto Object::add_child(std::unique_ptr<Object> x) -> void
	{
		if (x == nullptr)
		{
			return;
		}

		x->parent_ = this;
		auto& child = children_.emplace_back(std::move(x));
		child->on_added_(this);
		on_child_added_(child.get());
	}

	auto Object::create_child(std::string_view x) -> Object&
	{
		auto child = std::make_unique<Object>(engine());
		child->set_name(x);
		auto* ptr = child.get();
		add_child(std::move(child));
		return *ptr;
	}

	auto Object::remove() -> std::unique_ptr<Object>
	{
		if (parent_ == nullptr)
		{
			return nullptr;
		}

		auto it = std::ranges::find_if(parent_->children_, [this](const auto& o) -> auto { return this == o.get(); });

		if (it == std::end(parent_->children_))
		{
			return nullptr;
		}

		auto child = std::move(*it);
		auto* parent = parent_;
		parent_ = nullptr;
		parent->children_.erase(it);
		parent->on_child_removed_(child.get());
		on_removed_(parent);
		return child;
	}

	auto Object::children() const noexcept -> const std::vector<std::unique_ptr<Object>>&
	{
		return children_;
	}

	auto Object::find_child(std::string_view x) const -> Object*
	{
		const auto it = std::ranges::find_if(children_, [x](auto& o) -> auto { return x == o->get_name(); });

		if (it == std::end(children_))
		{
			return nullptr;
		}

		return (*it).get();
	}

	auto Object::parent() const noexcept -> Object*
	{
		return parent_;
	}

	auto Object::engine() const -> Engine&
	{
		return *engine_;
	}
}
