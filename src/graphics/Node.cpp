#include <druid/graphics/Node.h>
#include <algorithm>

namespace druid::graphics
{
	Node::Node()
	{
		on_added([this](auto* parent) { parent_node_ = dynamic_cast<Node*>(parent); });
		on_removed([this](auto*) { parent_node_ = nullptr; });
		on_child_added([this](auto* child) { nodes_.emplace_back(dynamic_cast<Node*>(child)); });
		on_child_removed([this](auto* child) { std::erase(nodes_, child); });
	}

	auto Node::set_position(const glm::vec2& pos) -> void
	{
		position_ = pos;
		update_transform();
	}

	auto Node::get_position() const -> glm::vec2
	{
		return position_;
	}

	auto Node::get_position_global() const -> glm::vec2
	{
		// NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
		const auto pos = glm::vec3(transform_global()[3]);

		// NOLINTNEXTLINE (cppcoreguidelines-pro-type-union-access,-warnings-as-errors)
		return {pos.x, pos.y};
	}

	auto Node::set_scale(const glm::vec2& scale) -> void
	{
		scale_ = scale;
		update_transform();
	}

	auto Node::get_scale() const -> glm::vec2
	{
		return scale_;
	}

	auto Node::set_rotation(float rotation) -> void
	{
		rotation_ = rotation;
		update_transform();
	}

	auto Node::get_rotation() const -> float
	{
		return rotation_;
	}

	auto Node::create_node() -> Node&
	{
		return create_node<Node>();
	}

	auto Node::nodes() -> std::vector<Node*>&
	{
		return nodes_;
	}

	auto Node::transform() const -> glm::mat4
	{
		return transform_;
	}

	auto Node::transform_global() const -> glm::mat4
	{
		if (parent() != nullptr)
		{
			// NOLINTNEXTLINE (cppcoreguidelines-pro-type-static-cast-downcast
			return parent_node_->transform_global() * transform();
		}

		return transform();
	}

	auto Node::draw(Renderer& x) const -> void
	{
		rlPushMatrix();
		rlMultMatrixf(glm::value_ptr(transform_));

		on_draw_(x);

		for (auto* node : nodes_)
		{
			node->draw(x);
		}

		rlPopMatrix();
	}

	auto Node::update_transform() -> void
	{
		// NOLINTBEGIN
		transform_ = glm::identity<glm::mat4>();
		transform_ = glm::translate(transform_, {position_.x, position_.y, 0.0F});
		transform_ = glm::rotate(transform_, glm::radians(rotation_), {1.0F, 0.0F, 0.0F});
		transform_ = glm::scale(transform_, {scale_.x, scale_.y, 1.0F});
		// NOLINTEND
	}
}