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

		auto insert_piece(ChessPiece chess_piece, ChessBoard::Square square)
		{
			// Place a piece on the square-centric board.
			chessboard_.at(magic_enum::enum_integer(square) - 1) = chess_piece;

			ChessBoard chessboard;
			chessboard.occupy_square(square);

			// if (chess_piece.piece_type() != ChessPiece::Type::Empty)
			// {
			// 	chessboards_by_color_.at(magic_enum::enum_integer(chess_piece.piece_color().value())).intersection_assign(chessboard);
			// 	chessboards_by_type_.at(magic_enum::integer)
			// }
		}

	private:
		/// @brief Square-centric board for "what piece is on square s?" queries
		std::array<ChessPiece, magic_enum::enum_count<ChessBoard::Square>()> chessboard_{};
		/// @brief Per piece-type board for queries such as, "where are all black knights?"
		std::array<ChessBoard, (magic_enum::enum_count<ChessPiece::Type>() * magic_enum::enum_count<ChessPiece::Color>())> chessboards_by_type_{};
		/// @brief Per color board for “which squares are occupied by side X?” queries
		std::array<ChessBoard, magic_enum::enum_count<ChessPiece::Color>()> chessboards_by_color_{};
	};
}