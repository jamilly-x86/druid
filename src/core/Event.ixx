module;

#include <cstdint>

export module druid.core.event;

namespace druid::core
{
	export class Event
	{
	public:
		enum class Type : std::uint8_t
		{
			Close
		};
	};
}