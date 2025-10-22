export module druid.core.entity;
export import <flecs.h>;

export namespace druid::core
{
	class Entity
	{
	public:
		explicit Entity( flecs::entity x) :  entity_{x}
		{
		}

		template <typename T, typename... Args>
		[[nodiscard]] auto add_component(Args&&... args) -> T&
		{
			// Construct a T from the forwarded args and call flecs::entity::set(T)
			// This ensures set() is called with a single value parameter (matching flecs overloads),
			// and supports the zero-arg case (default-constructed T).
			entity_.set<T>(T(std::forward<Args>(args)...));
			return entity_.get_mut<T>();
		}

		template <typename T>
		auto remove_component() -> void
		{
			entity_.remove<T>();
		}

		template <typename... Args>
		[[nodiscard]] auto has_components() const -> bool
		{
			return (entity_.has<Args>() && ...);
		}

		[[nodiscard]] auto valid() const -> bool
		{
			return entity_.is_valid();
		}

	private:
		flecs::entity entity_{};
	};
}