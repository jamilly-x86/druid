module;

export module druid.core.event;

namespace druid::core
{
	export class Event
	{
	public:
		enum class Type : int
		{
			Close
		};
	};
}