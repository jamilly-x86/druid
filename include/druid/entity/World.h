#pragma once

#include <cstdint>
#include <vector>

namespace druid::entity
{
	enum class Entity : std::uint64_t
	{
	};

	class World
	{
	public:
		[[nodiscard]] auto create_entity() -> Entity
		{
			const auto entity = static_cast<Entity>(entities_.size());
			entities_.emplace_back(entity);
			return entity;
		}

	private:
		std::vector<Entity> entities_;
	};
}