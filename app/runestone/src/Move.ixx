/// @file Move.ixx
/// @module runestone.move
module;

#include <cstdint>

export module runestone.move;

import runestone.chessboard;

export namespace runestone
{
	class Move
	{
	public:
		enum class Type : std::uint8_t
		{
			Normal = 0,
			Promotion = 1,
			EnPassant = 2,
			Castling = 3,
		};

		enum class Promo : std::uint8_t
		{
			Knight = 0,
			Bishop = 1,
			Rook = 2,
			Queen = 3,
		};

		struct Moving
		{
			ChessBoard::Square from;
			ChessBoard::Square to;
		};

		constexpr Move() noexcept = default;

		static constexpr auto make(Moving moving, Type type = Type::Normal, Promo promo = Promo::Queen) noexcept -> Move
		{
			Move move;
			move.data_ = (static_cast<std::uint16_t>(moving.to) & SquareMask) | ((static_cast<std::uint16_t>(moving.from) & SquareMask) << FromShift)
						 | ((static_cast<std::uint16_t>(promo) & TwoBitMask) << PromoShift)
						 | ((static_cast<std::uint16_t>(type) & TwoBitMask) << TypeShift);

			return move;
		}

	private:
		static constexpr std::uint16_t SquareMask = 0x3F;
		static constexpr std::uint16_t TwoBitMask = 0x3;
		static constexpr auto ToShift = 0;
		static constexpr auto FromShift = 6;
		static constexpr auto PromoShift = 12;
		static constexpr auto TypeShift = 14;
		std::uint16_t data_{0};
	};
}