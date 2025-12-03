#pragma once

#include <flecs.h>

namespace druid::anchor
{
	class Module
	{
	public:
		explicit Module(flecs::world& x);
	};
}