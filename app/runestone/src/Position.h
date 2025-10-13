#pragma once

#include <Types.h>

#include <array>
#include <string_view>

namespace runestone
{
	class Position
	{
	public:
		enum class FenStringFields : std::uint8_t
		{
			PiecePlacement = 0,
			SideToMove = 1,
			CastlingAbility = 2,
			EnPassantTargetSquare = 3,
			HalfmoveClock = 4,
			FullmoveCounter = 5
		};

		/// @brief Default constructor initializes an empty position
		Position() = default;

		/// @brief Set position from FEN (Forsyth-Edwards Notation) string
		///
		/// For more information on FEN strings, see https://www.chessprogramming.org/Forsyth-Edwards_Notation
		///
		/// @param fen_string Standard FEN string representing a chess position
		/// @return Reference to this position for method chaining
		///
		/// @note Assumes input FEN string is valid (as guaranteed by UCI-compliant GUIs).
		/// Parses all 6 FEN components: piece placement, side to move, castling rights,
		/// en passant square, half-move clock, and full-move number
		auto set_from_fen(std::string_view fen_string) -> void;

		auto create_piece(Piece piece, Square square) -> void;

		/// @brief Reset position to empty state with default game parameters
		///
		/// @note Clears all pieces from the board, resets bitboards, and sets:
		/// - Side to move: White
		/// - Castling rights: All available
		/// - En passant square: None
		/// - Half-move clock: 0
		/// - Full-move number: 1
		auto clear_position() -> void;

		/// @brief Set castling rights based on FEN character
		/// @param c FEN castling character ('K'=White kingside, 'Q'=White queenside,
		///          'k'=Black kingside, 'q'=Black queenside)
		///
		/// Uses bitwise OR to combine castling rights. Invalid characters are ignored.
		/// This function is typically called during FEN parsing to build up the
		/// complete castling rights from individual characters.
		// auto set_castling_rights(char c) -> void;

	private:
		/// @brief Square-centric board for "what piece is here?" queries
		std::array<Piece, static_cast<std::size_t>(Square::Size)> board_{Piece::Empty};
		std::array<BitBoard, static_cast<std::size_t>(PieceType::Size)> bitboard_by_piece_{};
		std::array<BitBoard, static_cast<std::size_t>(Color::Size)> bitboard_by_color_{};
		/// @brief For 50-move rule
		///
		/// @note game state variable
		int half_move_clock_{0};
		/// @brief Increments after black's move
		///
		/// @note game state variable
		int full_move_number_{1};
		Color side_to_move_{Color::White};
		CastlingRights castling_rights_{CastlingRights::All};
		/// @brief Invalid square = no en passant
		///
		/// @note game state variable
		Square enpassant_square_{Square::Size};
		FenStringFields current_fen_string_field_{FenStringFields::PiecePlacement};
	};
}