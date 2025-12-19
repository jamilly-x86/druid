#pragma once

#include <flecs.h>

namespace druid::anchor
{
	/// @class Module
	/// @brief Flecs module responsible for registering anchor-related components and systems.
	///
	/// `Module` encapsulates the registration of all ECS components, tags, and
	/// systems related to anchoring behavior within the Druid engine. Constructing
	/// this module typically:
	/// - Registers the `anchor` components (e.g., `Anchor`) with the Flecs world.
	/// - Sets up systems that resolve anchored positions each frame or during
	///   layout passes.
	/// - Establishes any required dependencies between anchor data and transform
	///   or layout systems.
	///
	/// This follows Flecs' module pattern, where simply instantiating the module
	/// performs all necessary registrations.
	class Module
	{
	public:
		/// @brief Construct and register the anchor module with a Flecs world.
		///
		/// The constructor is expected to perform all module initialization,
		/// including component registration and system setup.
		///
		/// @param x Flecs world into which the module is registered.
		explicit Module(flecs::world& x);
	};
}
