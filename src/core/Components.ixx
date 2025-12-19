/// @file Components.ixx
/// @module druid.core.components
module;

export module druid.core.components;

export namespace druid::core
{
	/// @struct Position
	/// @brief 2D position component.
	///
	/// Represents a point in 2D space using Cartesian coordinates.
	/// Commonly used as a component in ECS or as a lightweight value
	/// type for spatial calculations.
	struct Position
	{
		/// @brief X-coordinate.
		float x{};
		/// @brief Y-coordinate.
		float y{};
	};

	/// @struct Scale
	/// @brief 2D scale component.
	///
	/// Represents non-uniform scaling along the X and Y axes.
	struct Scale
	{
		/// @brief Scale factor along the X-axis.
		float x{1.0F};
		/// @brief Scale factor along the Y-axis.
		float y{1.0F};
	};

	/// @struct Size
	/// @brief 2D size component.
	///
	/// Represents width and height dimensions, commonly used for
	/// layout, bounding boxes, or UI elements.
	struct Size
	{
		/// @brief Width dimension.
		float width{};
		/// @brief Height dimension.
		float height{};
	};

	/// @struct Transform
	/// @brief Composite 2D transform component.
	///
	/// Combines position, rotation, and scale into a single structure.
	/// Useful for ECS systems or scene graph integration where a unified
	/// transform representation is desirable.
	struct Transform
	{
		/// @brief Local position.
		Position position{};
		/// @brief Local rotation angle.
		///
		/// @note Units (radians vs degrees) are determined by the consuming system
		///       and should be used consistently across the engine.
		float rotation{};
		/// @brief Local scale.
		Scale scale{};
	};
}
