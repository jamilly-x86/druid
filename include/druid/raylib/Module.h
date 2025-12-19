#pragma once

#include <flecs.h>

namespace druid::raylib
{
	/// @class Module
	/// @brief Flecs module for integrating Raylib with the Druid engine.
	///
	/// `Module` encapsulates all Raylib-specific ECS registrations, including
	/// components, systems, and initialization hooks required to use Raylib as
	/// a rendering or windowing backend within a Flecs world.
	///
	/// Constructing this module typically:
	/// - Registers Raylib-related components and tags.
	/// - Sets up systems that bridge ECS data (transforms, drawables, etc.) to
	///   Raylib rendering calls.
	/// - Initializes or configures Raylib-specific runtime behavior as needed.
	///
	/// This class follows the Flecs module pattern, where instantiating the module
	/// performs all necessary registration and setup.
	class Module
	{
	public:
		/// @brief Construct and register the Raylib module with a Flecs world.
		///
		/// The constructor is expected to perform all module initialization,
		/// including component registration and system setup.
		///
		/// @param x Flecs world into which the Raylib module is registered.
		explicit Module(flecs::world& x);
	};
}
