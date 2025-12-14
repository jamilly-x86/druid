/// @file Position.ixx
/// @module runestone.position
module;

#include <array>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

export module runestone.position;

import runestone.chessboard;
import runestone.chesspiece;

export namespace runestone
{
	class Position
	{
	public:
		Position() noexcept = default;

	private:
		std::array<ChessPiece, magic_enum::enum_count<ChessBoard::Square>()> chessboard_{};
		std::array<ChessBoard, (magic_enum::enum_count<ChessPiece::Type>() * magic_enum::enum_count<ChessPiece::Color>()) + 2> chessboard_by_type_{};
		std::array<ChessBoard, magic_enum::enum_count<ChessPiece::Color>()> chessboards_by_color_{};
	};
}