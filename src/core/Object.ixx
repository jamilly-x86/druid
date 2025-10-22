module;

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

export module druid.core.object;
import druid.core.signal;

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
			on_destroyed_();
		}

		Object(const Object&) = delete;
		auto operator=(const Object&) -> Object& = delete;
		Object(Object&&) noexcept = delete;
		auto operator=(Object&&) noexcept -> Object& = delete;

		/// @brief Assign an identifying name to this object.
		/// @param x The given name to assign to this object.
		auto set_name(std::string_view x) -> void
		{
			this->name_ = x;
		}

		/// @brief Get the name assigned to this object.
		/// @return The name assigned to this object.
		[[nodiscard]] auto get_name() const noexcept -> std::string_view
		{
			return this->name_;
		}

		/// @brief Add the given object as a child of this object.
		/// @param x The given object to add as a child.
		auto add_child(std::unique_ptr<Object> x) -> void
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

		/// @brief Create an object as a child of this object and return a reference to it.
		/// @param x The given name to assign to this object.
		/// @return The reference to the created child.
		[[nodiscard]] auto create_child(std::string_view x = {}) -> Object&
		{
			auto child = std::make_unique<Object>();
			child->set_name(x);
			auto* ptr = child.get();
			add_child(std::move(child));
			return *ptr;
		}

		/// @brief Create an object of type T as a child of this object and return a reference to it.
		/// @tparam ...Args The constructor argument types of T.
		/// @tparam T The type of object to create. Must inherit from Object.
		/// @param ...args The constructor arguments to forward to T's constructor.
		/// @return The reference to the created child.
		template <ObjectType T, typename... Args>
		[[nodiscard]] auto create_child(Args&&... args) -> T&
		{
			auto child = std::make_unique<T>(std::forward<Args>(args)...);
			auto* ptr = child.get();
			add_child(std::move(child));
			return *ptr;
		}

		/// @brief Remove this object from its parent if it has one.
		/// @return Return the owning unique_ptr of this object.
		[[nodiscard]] auto remove() -> std::unique_ptr<Object>
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

		/// @brief Get the list of children this object owns.
		/// @return The, read-only, list of children.
		[[nodiscard]] auto children() const noexcept -> const std::vector<std::unique_ptr<Object>>&
		{
			return children_;
		}

		/// @brief Find a child with the given name.
		/// @param x The name to search this object's children for.
		/// @return The object whose name matches the given name.
		[[nodiscard]] auto find_child(std::string_view x) const -> Object*
		{
			const auto it = std::ranges::find_if(children_, [x](auto& o) -> auto { return x == o->get_name(); });

			if (it == std::end(children_))
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

		auto on_destroyed(auto x) -> void
		{
			on_destroyed_.connect(std::forward<decltype(x)>(x));
		}

		auto on_added(auto x) -> void
		{
			on_added_.connect(std::forward<decltype(x)>(x));
		}

		auto on_removed(auto x) -> void
		{
			on_removed_.connect(std::forward<decltype(x)>(x));
		}

		auto on_child_added(auto x) -> void
		{
			on_child_added_.connect(std::forward<decltype(x)>(x));
		}

		auto on_child_removed(auto x) -> void
		{
			on_child_removed_.connect(std::forward<decltype(x)>(x));
		}

	private:
		std::vector<std::unique_ptr<Object>> children_;
		std::string name_;
		Object* parent_{};

		Signal<void()> on_destroyed_;
		Signal<void(Object*)> on_added_;
		Signal<void(Object*)> on_removed_;
		Signal<void(Object*)> on_child_added_;
		Signal<void(Object*)> on_child_removed_;
	};
}