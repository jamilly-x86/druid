module;

#include <algorithm>
#include <memory>
#include <sigslot/signal.hpp>
#include <string>
#include <string_view>
#include <vector>

export module druid.core.object;

export namespace druid::core
{
	class Object;
	template <typename T>
	concept ObjectType = std::is_base_of_v<Object, T>;

	/// @brief The base class of druid. This object manages child-parent relationship hierarchies.
	class Object
	{
	public:
		Object() = default;

		virtual ~Object()
		{
			on_destroyed();
		}

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

			x->parent_ = this;
			auto& child = children_.emplace_back(std::move(x));
			child->on_added(this);
			on_child_added(child.get());
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

		/// @brief Create an object of type T as a child of this object and return a reference to it.
		/// @tparam ...Args The constructor argument types of T.
		/// @tparam T The type of object to create. Must inherit from Object.
		/// @param ...args The constructor arguments to forward to T's constructor.
		/// @return The reference to the created child.
		template <ObjectType T, typename... Args>
		[[nodiscard]] auto createChild(Args&&... args) -> T&
		{
			auto child = std::make_unique<T>(std::forward<Args>(args)...);
			auto* ptr = child.get();
			this->addChild(std::move(child));
			return *ptr;
		}

		/// @brief Remove this object from its parent if it has one.
		/// @return Return the owning unique_ptr of this object.
		[[nodiscard]] auto remove() -> std::unique_ptr<Object>
		{
			if(parent_ == nullptr)
			{
				return nullptr;
			}

			auto it = std::ranges::find_if(parent_->children_, [this](const auto& o) -> auto { return this == o.get(); });

			if(it == std::end(parent_->children_))
			{
				return nullptr;
			}

			auto child = std::move(*it);
			auto parent = parent_;
			parent_ = nullptr;
			parent->children_.erase(it);
			parent->on_child_removed(child.get());
			on_removed(parent);
			return child;
		}

		/// @brief Get the list of children this object owns.
		/// @return The, read-only, list of children.
		[[nodiscard]] auto children() const noexcept -> const std::vector<std::unique_ptr<Object>>&
		{
			return children_;
		}

		/// @brief Find a child with the given name.
		/// @param x The name to search this object's children for.
		/// @return The object whose name matches the given name.
		[[nodiscard]] auto findChild(std::string_view x) const -> Object*
		{
			const auto it = std::ranges::find_if(children_, [x](auto& o) -> auto { return x == o->getName(); });

			if(it == std::end(children_))
			{
				return nullptr;
			}

			return (*it).get();
		}

		/// @brief Returns a pointer to the parent object.
		/// @return A pointer to the parent Object, or nullptr if there is no parent.
		[[nodiscard]] auto parent() const noexcept -> Object*
		{
			return parent_;
		}

		sigslot::signal<> on_destroyed;
		sigslot::signal<Object*> on_added;
		sigslot::signal<Object*> on_removed;
		sigslot::signal<Object*> on_child_added;
		sigslot::signal<Object*> on_child_removed;

	private:
		std::vector<std::unique_ptr<Object>> children_;
		std::string name;
		Object* parent_{};
	};
}