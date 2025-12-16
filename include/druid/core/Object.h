#pragma once

#include <druid/core/Signal.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <gsl/pointers>

namespace druid::core
{
	class Engine;
	
	class Object;

	/// @concept ObjectType
	/// @brief Constrains a type to those derived from druid::core::Object.
	template <typename T>
	concept ObjectType = std::is_base_of_v<Object, T>;

	/// @class Object
	/// @brief Base class for all hierarchical objects in the Druid engine.
	///
	/// `Object` provides a scene-graph–like parent/child hierarchy with ownership
	/// semantics based on `std::unique_ptr`. Each object:
	/// - Is owned either by an engine or another object.
	/// - May own zero or more child objects.
	/// - Knows its parent (if any).
	/// - Can emit lifecycle signals when it is added, removed, or destroyed.
	///
	/// Objects are non-copyable and non-movable to preserve hierarchy integrity.
	class Object
	{
	public:
		/// @brief Construct an object associated with the given engine.
		/// @param x Owning engine instance.
		Object(Engine& x);

		/// @brief Virtual destructor.
		///
		/// Emits the `on_destroyed` signal before destruction.
		virtual ~Object() noexcept;

		Object(const Object&) = delete;
		auto operator=(const Object&) -> Object& = delete;
		Object(Object&&) noexcept = delete;
		auto operator=(Object&&) noexcept -> Object& = delete;

		/// @brief Assign an identifying name to this object.
		/// @param x The given name to assign to this object.
		auto set_name(std::string_view x) -> void;

		/// @brief Get the name assigned to this object.
		/// @return The name assigned to this object.
		[[nodiscard]] auto get_name() const noexcept -> std::string_view;

		/// @brief Add the given object as a child of this object.
		/// @param x The given object to add as a child.
		auto add_child(std::unique_ptr<Object> x) -> void;

		/// @brief Create an object as a child of this object and return a reference to it.
		/// @param x The given name to assign to this object.
		/// @return The reference to the created child.
		[[nodiscard]] auto create_child(std::string_view x = {}) -> Object&;

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
		[[nodiscard]] auto remove() -> std::unique_ptr<Object>;

		/// @brief Get the list of children this object owns.
		/// @return The, read-only, list of children.
		[[nodiscard]] auto children() const noexcept -> const std::vector<std::unique_ptr<Object>>&;

		/// @brief Find a child with the given name.
		/// @param x The name to search this object's children for.
		/// @return The object whose name matches the given name.
		[[nodiscard]] auto find_child(std::string_view x) const -> Object*;

		/// @brief Returns a pointer to the parent object.
		/// @return A pointer to the parent Object, or nullptr if there is no parent.
		[[nodiscard]] auto parent() const noexcept -> Object*;

		/// @brief Subscribe to the destroyed signal.
		///
		/// This signal is emitted immediately before this object is destroyed.
		/// Callbacks are invoked while the object is still valid, allowing
		/// observers to perform cleanup or detach references.
		///
		/// @tparam Callback A callable type invocable with no arguments.
		/// @param x Callback function to invoke when this object is about to be destroyed.
		template <typename Callback>
		auto on_destroyed(Callback&& x) -> void
		{
			on_destroyed_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe to the removed-from-parent signal.
		///
		/// This signal is emitted when this object is detached from its parent.
		/// The callback is invoked with a pointer to the object that was removed
		/// (i.e., `this`).
		///
		/// @tparam Callback A callable type invocable with `Object*`.
		/// @param x Callback function to invoke when this object is removed
		///          from its parent.
		template <typename Callback>
		auto on_added(Callback&& x) -> void
		{
			on_added_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe to the child-added signal.
		///
		/// This signal is emitted whenever a new child object is added to this
		/// object. The callback receives a pointer to the child that was added.
		///
		/// @tparam Callback A callable type invocable with `Object*`.
		/// @param x Callback function to invoke when a child is added.
		template <typename Callback>
		auto on_removed(Callback&& x) -> void
		{
			on_removed_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe to the child-removed signal.
		///
		/// This signal is emitted whenever a child object is removed from this
		/// object. The callback receives a pointer to the child that was removed.
		///
		/// @tparam Callback A callable type invocable with `Object*`.
		/// @param x Callback function to invoke when a child is removed.
		template <typename Callback>
		auto on_child_added(Callback&& x) -> void
		{
			on_child_added_.connect(std::forward<Callback>(x));
		}

		/// @brief Subscribe to the child-removed signal.
		///
		/// This signal is emitted whenever a child object is removed from this
		/// object. The callback receives a pointer to the child that was removed.
		///
		/// @tparam Callback A callable type invocable with `Object*`.
		/// @param x Callback function to invoke when a child is removed.
		template <typename Callback>
		auto on_child_removed(Callback&& x) -> void
		{
			on_child_removed_.connect(std::forward<Callback>(x));
		}

	protected:
		/// @brief Access the owning engine.
		/// @return Reference to the engine associated with this object.
		[[nodiscard]] auto engine() const -> Engine&;

	private:
		std::vector<std::unique_ptr<Object>> children_;
		std::string name_;
		Object* parent_{};
		gsl::strict_not_null<Engine*> engine_;

		Signal<void()> on_destroyed_;
		Signal<void(Object*)> on_added_;
		Signal<void(Object*)> on_removed_;
		Signal<void(Object*)> on_child_added_;
		Signal<void(Object*)> on_child_removed_;
	};
}