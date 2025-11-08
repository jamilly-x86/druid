#pragma once

#include <druid/core/Object.h>
#include <druid/core/Signal.h>
#include <druid/graphics/Renderer.h>
#include <raylib.h>
#include <rlgl.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <vector>

using druid::core::Signal;

namespace druid::graphics
{
	class Node;

	template <typename T>
	concept NodeType = std::is_base_of_v<Node, T>;

	class Node : public druid::core::Object
	{
	public:
		static constexpr auto DefaultPosition = glm::vec2{0.0F, 0.0F};
		static constexpr auto DefaultScale = glm::vec2{1.0F, 1.0F};
		static constexpr auto DefaultRotation = 0.0F;

		Node(druid::core::Engine& x);

		auto set_position(const glm::vec2& pos) -> void;

		[[nodiscard]] auto get_position() const -> glm::vec2;

		[[nodiscard]] auto get_position_global() const -> glm::vec2;

		auto set_scale(const glm::vec2& scale) -> void;

		[[nodiscard]] auto get_scale() const -> glm::vec2;

		auto set_rotation(float rotation) -> void;

		[[nodiscard]] auto get_rotation() const -> float;

		template <NodeType T, typename... Args>
		[[nodiscard]] auto create_node(Args&&... args) -> T&
		{
			auto child = std::make_unique<T>(engine(), std::forward<Args>(args)...);
			auto* ptr = child.get();
			add_child(std::move(child));
			return *ptr;
		}

		[[nodiscard]] auto create_node() -> Node&;

		[[nodiscard]] auto nodes() -> std::vector<Node*>&;

		[[nodiscard]] auto transform() const -> glm::mat4;

		[[nodiscard]] auto transform_global() const -> glm::mat4;

		auto draw(Renderer& x) const -> void;

		template <typename Callback>
		auto on_draw(Callback&& x) -> void
		{
			on_draw_.connect(std::forward<Callback>(x));
		}

	private:
		// Hide Object::create_child to only allow Node creation.
		using Object::create_child;

		auto update_transform() -> void;

		std::vector<Node*> nodes_;
		Node* parent_node_{nullptr};

		Signal<void(Renderer&)> on_draw_;

		glm::mat4 transform_{glm::identity<glm::mat4>()};
		glm::vec2 position_{DefaultPosition};
		glm::vec2 scale_{DefaultScale};
		float rotation_{DefaultRotation};
	};
}