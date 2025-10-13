#include <Position.h>

#include <BitBoard.h>
#include <Utils.h>

using runestone::Position;

auto Position::set_from_fen(std::string_view fen_string) -> void
{
	clear_position();

	auto current_square_centric_index = 0;
	for (const auto c : fen_string)
	{
		// fen strings are space delimited. if a space is encountered,
		// move to next field in FenStringFields enum class
		if (c != ' ')
		{
			switch (current_fen_string_field_)
			{
				case FenStringFields::PiecePlacement:
				{
					if (c != '/')
					{
						create_piece(utils::CharToPiece(c), static_cast<Square>(current_square_centric_index));
						// if the character is a digit, increment the current square centric index
						// by the value of the character. else increment by one (i.e. a piece was created)
						current_square_centric_index += (std::isdigit(c) == 1) ? c - '0' : 1;
					}
					break;
				}
				case FenStringFields::SideToMove:
				{
					side_to_move_ = (c == 'w') ? Color::White : Color::Black;
					break;
				}
				case FenStringFields::CastlingAbility:
				{

					break;
				}
				default:
				{
				}
			}
		}
	}
}

auto Position::create_piece(Piece piece, Square square) -> void
{
	// update the square-centric board
	// NOLINTNEXTLINE (cppcoreguidelines-pro-bounds-constant-array-index)
	board_[static_cast<std::size_t>(square)] = piece;

	if (piece == Piece::Empty)
	{
		return;
	}

	const auto square_bitboard = bitboard::SquareBitBoard(square);

	// extract type and color using bit masks
	const auto piece_type = piece_encoding::GetPieceType(piece);
	const auto color = piece_encoding::GetPieceColor(piece);
	const auto color_index = static_cast<std::size_t>(color) >> 3U;

	// update bitboards maintaining color and piece
	// NOLINTBEGIN (cppcoreguidelines-pro-bounds-constant-array-index)
	bitboard_by_piece_[static_cast<std::size_t>(piece_type)] |= square_bitboard;
	bitboard_by_color_[color_index] |= square_bitboard;
	// NOLINTEND
}

auto Position::clear_position() -> void
{
	// reset all bitboards
	board_.fill(Piece::Empty);
	bitboard_by_piece_.fill(0ULL);
	bitboard_by_color_.fill(0ULL);

	// reset all game state variables
	half_move_clock_ = 0;
	full_move_number_ = 1;
	side_to_move_ = Color::White;
	castling_rights_ = CastlingRights::All;
	enpassant_square_ = Square::Size;

	// update fen string field for parsing
	current_fen_string_field_ = FenStringFields::PiecePlacement;
}

// auto Position::set_castling_rights(char c) -> void
// {
// 	switch(c)
// 	{
// 		case 'K':
// 		{
// 			castling_rights_ |= CastlingRights::WhiteKingSide;
// 			break;
// 		}
// 		case 'Q':
// 		{
// 			castling_rights_ |= CastlingRights::WhiteQueenSide;
// 			break;
// 		}
// 		case 'k':
// 		{
// 			castling_rights_ |= CastlingRights::BlackKingSide;
// 			break;
// 		}
// 		case 'q':
// 		{
// 			castling_rights_ |= CastlingRights::BlackQueenSide;
// 			break;
// 		}
// 		default:
// 		{
// 			castling_rights_ = CastlingRights::None;
// 			break;
// 		}
// 	}
// }

