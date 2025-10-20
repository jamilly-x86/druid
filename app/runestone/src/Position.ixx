module;

#include <array>
#include <string_view>

export module runestone.position;

import runestone.chessboard;
import runestone.utils;
import runestone.piecefactory;

export namespace runestone
{
	class Position
	{
	public:
		Position() noexcept = default;

		auto insert_piece(PieceFactory::Piece piece, ChessBoard::Square square) -> void
		{
			chessboards_[static_cast<std::size_t>(square)] = piece;

			ChessBoard chessboard;
			chessboard.occupy_square(square);

			const auto piece_type = PieceFactory::piece_type(piece);
			const auto piece_color = PieceFactory::piece_color(piece);
			const auto color_index = static_cast<std::size_t>(piece_color.value()) >> 3U;

			chessboards_by_piece_[static_cast<std::size_t>(piece_type.value())].intersection_assign(chessboard);
			chessboards_by_color_[color_index].intersection_assign(chessboard);
		}

		// auto erase_piece(PieceFactory::Piece piece, ChessBoard::Square square) -> void
		// {
		// 	// not sure if i need to pass the actual piece when deleting it from the position...will
		// 	// need to think about system architectuer more
		// }

		// auto fen_to_position(std::string_view fen_string) -> void
		// {
		// 	clear();
		//
		// 	auto fen_string_index = 0;
		// 	for (auto c : fen_string)
		// 	{
		// 		if (c == ' ')
		// 		{
		// 			break;
		// 		}
		// 	}
		// }
		//
		// auto position_to_fen() -> std::string_view
		// {
		// 	return " ";
		// }

		auto clear() -> void
		{
			chessboards_.fill(PieceFactory::Piece::Empty);
			chessboards_by_color_.fill(std::array<ChessBoard, utils::ToIndex<PieceFactory::Color>()>::value_type(0ULL));
			chessboards_by_piece_.fill(std::array<ChessBoard, utils::ToIndex<PieceFactory::Piece>()>::value_type(0ULL));

			half_move_clock_ = 0;
			full_move_number_ = 1;
			side_to_move_ = PieceFactory::Color::White;
			enpassant_square_ = ChessBoard::Square::Size;
		}

	private:
		/// @brief Square-centric board for "what piece is here?" queries
		std::array<ChessBoard, utils::ToIndex<PieceFactory::Piece>()> chessboards_by_piece_{};
		std::array<PieceFactory::Piece, utils::ToIndex<ChessBoard::Square>()> chessboards_{PieceFactory::Piece::Empty};
		std::array<ChessBoard, utils::ToIndex<PieceFactory::Color>()> chessboards_by_color_{};

		int half_move_clock_{0};
		int full_move_number_{1};
		PieceFactory::Color side_to_move_{PieceFactory::Color::White};
		// CastlingRights castling_rights_{runestone::CastlingRights::None};
		ChessBoard::Square enpassant_square_{ChessBoard::Square::Size};
	};
}