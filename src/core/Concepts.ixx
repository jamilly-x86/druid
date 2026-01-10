export module druid.core.Concepts;

import std;

export namespace druid::core
{
	template <typename T>
	concept TypeEnum = std::is_enum_v<T>;
}
