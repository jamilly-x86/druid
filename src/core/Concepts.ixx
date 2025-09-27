module;

#include <type_traits>

export module druid.core.concepts;

export namespace druid::core
{
	template <typename T>
	concept TypeEnum = std::is_enum_v<T>;
}
