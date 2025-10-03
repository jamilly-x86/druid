module;

#include <raylib.h>
#include <rlgl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <vector>

// Don't remove. Required to fix gcc compiler error.
#include <typeinfo>

export module druid.graphics.node;
import druid.core.object;

using druid::core::Signal;

namespace druid::graphics
{
	export class Node : public druid::core::Object
	{
	public:
		static constexpr auto DefaultPosition = glm::vec2{0.0F, 0.0F};
		static constexpr auto DefaultScale = glm::vec2{1.0F, 1.0F};
		static constexpr auto DefaultRotation = 0.0F;

		Node()
		{
			on_added([this](auto* parent) { parent_node_ = dynamic_cast<Node*>(parent); });
			on_removed([this](auto*) { parent_node_ = nullptr; });
			on_child_added([this](auto* child) { nodes_.emplace_back(dynamic_cast<Node*>(child)); });
			on_child_removed([this](auto* child) { std::erase(nodes_, child); });
		}

		auto set_position(const glm::vec2& pos) -> void
		{
			position_ = pos;
			update_transform();
		}

		[[nodiscard]] auto get_position() const -> glm::vec2
		{
			return position_;
		}

		[[nodiscard]] auto get_position_global() const -> glm::vec2
		{
			// NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-avoid-unchecked-container-access)
			const auto pos = glm::vec3(transform_global()[3]);

			// NOLINTNEXTLINE (cppcoreguidelines-pro-type-union-access,-warnings-as-errors)
			return {pos.x, pos.y};
		}

		auto set_scale(const glm::vec2& scale) -> void
		{
			scale_ = scale;
			update_transform();
		}

		[[nodiscard]] auto get_scale() const -> glm::vec2
		{
			return scale_;
		}

		auto set_rotation(float angle) -> void
		{
			rotation_ = angle;
			update_transform();
		}

		[[nodiscard]] auto get_rotation() const -> float
		{
			return rotation_;
		}

		[[nodiscard]] auto create_node() -> Node&
		{
			return create_child<Node>();
		}

		[[nodiscard]] auto parent_node() const noexcept -> Node*
		{
			return parent_node_;
		}

		[[nodiscard]] auto nodes() const noexcept -> const std::vector<Node*>&
		{
			return nodes_;
		}

		auto transform() const -> glm::mat4
		{
			return transform_;
		}

		auto transform_global() const -> glm::mat4
		{
			if (parent() != nullptr)
			{
				// NOLINTNEXTLINE (cppcoreguidelines-pro-type-static-cast-downcast
				return static_cast<Node*>(parent())->transform_global() * transform();
			}

			return transform();
		}

		auto draw() const -> void
		{
			rlPushMatrix();
			rlMultMatrixf(glm::value_ptr(transform_));

			on_draw_();

			for (auto* node : nodes_)
			{
				node->draw();
			}

			rlPopMatrix();
		}

		auto on_draw(auto x) -> void
		{
			on_draw_.connect(std::forward<decltype(x)>(x));
		}

	private:
		// Hide Object::create_child to only allow Node creation.
		using Object::create_child;

		auto update_transform() -> void
		{
			// NOLINTBEGIN
			transform_ = glm::identity<glm::mat4>();
			transform_ = glm::translate(transform_, {position_.x, position_.y, 0.0F});
			transform_ = glm::rotate(transform_, glm::radians(rotation_), {0.0F, 0.0F, 1.0F});
			transform_ = glm::scale(transform_, {scale_.x, scale_.y, 1.0F});
			// NOLINTEND
		}

		std::vector<Node*> nodes_;
		Node* parent_node_{nullptr};

		Signal<> on_draw_;

		glm::mat4 transform_{glm::identity<glm::mat4>()};
		glm::vec2 position_{DefaultPosition};
		glm::vec2 scale_{DefaultScale};
		float rotation_{DefaultRotation};
	};
}