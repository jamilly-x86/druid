module;

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

export module druid.core.object;

export namespace druid::core
{
	/// @brief The base class of druid. This object manages child-parent relationship hierarchies.
	class Object
	{
	public:
		Object() = default;
		virtual ~Object() = default;
		Object(const Object&) = delete;
		auto operator=(const Object&) -> Object& = delete;
		Object(Object&&) noexcept = delete;
		auto operator=(Object&&) noexcept -> Object& = delete;

		/// @brief Assign an identifying name to this object.
		/// @param x The given name to assign to this object.
		auto setName(std::string_view x) -> void
		{
			this->name = x;
		}

		/// @brief Get the name assigned to this object.
		/// @return The name assigned to this object.
		[[nodiscard]] auto getName() const noexcept -> std::string_view
		{
			return this->name;
		}

		/// @brief Add the given object as a child of this object.
		/// @param x The given object to add as a child.
		auto addChild(std::unique_ptr<Object> x) -> void
		{
			if(x == nullptr)
			{
				return;
			}

			x->parent = this;
			this->children.emplace_back(std::move(x));
		}

		/// @brief Create an object as a child of this object and return a reference to it.
		/// @param x The given name to assign to this object.
		/// @return The reference to the created child.
		[[nodiscard]] auto createChild(std::string_view x = {}) -> Object&
		{
			auto child = std::make_unique<Object>();
			child->setName(x);
			auto* ptr = child.get();
			this->addChild(std::move(child));
			return *ptr;
		}

		/// @brief Remove this object from its parent if it has one.
		/// @return Return the owning unique_ptr of this object.
		[[nodiscard]] auto remove() -> std::unique_ptr<Object>
		{
			if(this->parent == nullptr)
			{
				return nullptr;
			}

			auto it = std::ranges::find_if(this->parent->children, [this](const auto& o) -> auto { return this == o.get(); });

			if(it == std::end(this->parent->children))
			{
				return nullptr;
			}

			auto child = std::move(*it);
			this->parent->children.erase(it);
			this->parent = nullptr;
			return child;
		}

		/// @brief Get the list of children this object owns.
		/// @return The, read-only, list of children.
		[[nodiscard]] auto getChildren() const noexcept -> const std::vector<std::unique_ptr<Object>>&
		{
			return this->children;
		}

		/// @brief Find a child with the given name.
		/// @param x The name to search this object's children for.
		/// @return The object whose name matches the given name.
		[[nodiscard]] auto findChild(std::string_view x) const -> Object*
		{
			const auto it = std::ranges::find_if(this->children, [x](auto& o) -> auto { return x == o->getName(); });

			if(it == std::end(this->children))
			{
				return nullptr;
			}

			return (*it).get();
		}

	private:
		std::vector<std::unique_ptr<Object>> children;
		std::string name;
		Object* parent{};
	};
}