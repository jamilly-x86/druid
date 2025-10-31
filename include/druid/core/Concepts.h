#pragma once

#include <type_traits>

namespace druid::core
{
	template <typename T>
	concept TypeEnum = std::is_enum_v<T>;
}