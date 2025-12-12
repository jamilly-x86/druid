module;

#include <cstdint>
#include <expected>

export module runestone.chesspiece;

export namespace runestone
{
	class ChessPiece
	{
	public:
		/// @enum Type
		/// @brief Represents the type of chess piece (color-agnostic).
		/// @note Piece type stored in bits 0-2 (0x7).
		enum class Type : std::uint8_t
		{
			// clang-format off
			Empty = 0,		// 0b000
			Pawn = 1,		// 0b001
			Knight = 2,		// 0b010
			Bishop = 3,		// 0b011
			Rook = 4,		// 0b100
			Queen = 5,		// 0b101
			King = 6,		// 0b110
					// clang-format on
		};

		/// @enum Color
		/// @brief Represents the color of a chess piece or player.
		/// @note Color is stored in bit 3 (0x8).
		enum class Color : std::uint8_t
		{
			// clang-format off
			White = 0,		// binary: 0b0000 = bit 3 is 0
			Black = 8,		// binary: 0b1000 = bit 3 is 1
					 // clang-format on
		};

		/// @enum Error
		/// @brief Represents a specific error thrown when calling chess piece utility functions.
		enum class Error : std::uint8_t
		{
			InvalidCharacter,
			InvalidPiece
		};

		/// @brief Convert a character to chess piece.
		/// @param c Character representing a piece ('K'=white king, 'k'=black king, etc.).
		/// @return Corresponding Piece enum value, or Error::InvalidCharacter.
		static constexpr auto char_to_piece(char c) noexcept -> std::expected<ChessPiece, Error>
		{
			switch (c)
			{
				case 'P':
				{
					return ChessPiece(Color::White, Type::Pawn);
				}
				case 'B':
				{
					return ChessPiece(Color::White, Type::Bishop);
				}
				case 'N':
				{
					return ChessPiece(Color::White, Type::Knight);
				}
				case 'R':
				{
					return ChessPiece(Color::White, Type::Rook);
				}
				case 'Q':
				{
					return ChessPiece(Color::White, Type::Queen);
				}
				case 'K':
				{
					return ChessPiece(Color::White, Type::King);
				}
				case 'p':
				{
					return ChessPiece(Color::Black, Type::Pawn);
				}
				case 'b':
				{
					return ChessPiece(Color::Black, Type::Bishop);
				}
				case 'n':
				{
					return ChessPiece(Color::Black, Type::Knight);
				}
				case 'r':
				{
					return ChessPiece(Color::Black, Type::Rook);
				}
				case 'q':
				{
					return ChessPiece(Color::Black, Type::Queen);
				}
				case 'k':
				{
					return ChessPiece(Color::Black, Type::King);
				}
				default:
				{
					return std::unexpected(Error::InvalidCharacter);
				}
			}
		}

		/// @brief Convert chess piece enum to FEN character.
		/// @param chess_piece Piece enum value to convert.
		/// @return Corresponding FEN character ('K', 'k', 'P', 'p', etc.) or Error::InvalidPiece.
		static constexpr auto piece_to_char(ChessPiece chess_piece) noexcept -> std::expected<char, Error>
		{
			switch (chess_piece.raw())
			{
				case UnderlyingType::WhitePawn:
				{
					return 'P';
				}
				case UnderlyingType::WhiteBishop:
				{
					return 'B';
				}
				case UnderlyingType::WhiteKnight:
				{
					return 'N';
				}
				case UnderlyingType::WhiteRook:
				{
					return 'R';
				}
				case UnderlyingType::WhiteQueen:
				{
					return 'Q';
				}
				case UnderlyingType::WhiteKing:
				{
					return 'K';
				}
				case UnderlyingType::BlackPawn:
				{
					return 'p';
				}
				case UnderlyingType::BlackBishop:
				{
					return 'b';
				}
				case UnderlyingType::BlackKnight:
				{
					return 'n';
				}
				case UnderlyingType::BlackRook:
				{
					return 'r';
				}
				case UnderlyingType::BlackQueen:
				{
					return 'q';
				}
				case UnderlyingType::BlackKing:
				{
					return 'k';
				}
				default:
				{
					return std::unexpected(Error::InvalidPiece);
				}
			}
		}

		/// @brief
		/// @param
		/// @param
		explicit ChessPiece(Color color, Type type)
		{
			if (type == Type::Empty)
			{
				underlying_type_ = UnderlyingType::Empty;
			}
			else
			{
				underlying_type_ = static_cast<UnderlyingType>(static_cast<uint8_t>(type) | static_cast<uint8_t>(color));
			}
		}

		auto operator==(const ChessPiece& chess_piece) const -> bool
		{
			return raw() == chess_piece.raw() && raw_bits() == chess_piece.raw_bits();
		}

		/// @brief Get the chess piece type.
		/// @return The chess piece type.
		[[nodiscard]] constexpr auto piece_type() const noexcept -> Type
		{
			return static_cast<Type>(raw_bits() & HexSeven);
		}

		/// @brief Get the chess piece color.
		/// @return The chess piece color.
		[[nodiscard]] constexpr auto piece_color() const noexcept -> std::expected<Color, Error>
		{
			if ((raw_bits() & HexSeven) == 0)
			{
				return std::unexpected(Error::InvalidPiece);
			}
			return static_cast<Color>(raw_bits() & HexEight);
		}

		/// @brief Check if piece is white.
		/// @return True if white, false otherwise. If piece is empty,
		/// return Error::InvalidPiece.
		[[nodiscard]] constexpr auto white() const noexcept -> std::expected<bool, Error>
		{
			if ((raw_bits() & HexSeven) == 0)
			{
				return std::unexpected(Error::InvalidPiece);
			}
			return (raw_bits() & HexEight) == 0;
		}

		/// @brief Check if piece is black.
		/// @return True if black, false otherwise. If piece is empty,
		/// return Error::InvalidPiece.
		[[nodiscard]] constexpr auto black() const noexcept -> std::expected<bool, Error>
		{
			if ((raw_bits() & HexSeven) == 0)
			{
				return std::unexpected(Error::InvalidPiece);
			}
			return (raw_bits() & HexEight) != 0;
		}

	private:
		/// @enum UnderlyingType
		/// @brief Represents a specific chess piece (combines color and type).
		/// @note Piece = Color (bit 3) | Type (bits 0-2).
		enum class UnderlyingType : std::uint8_t
		{
			// clang-format off
			Empty = 0,
			// white pieces (color = 0, so bit 3 = 0)
			WhitePawn = 1,		// 0b0001 = 0 (white) | 1 (pawn)
			WhiteKnight = 2,	// 0b0010 = 0 (white) | 2 (knight)
			WhiteBishop = 3,	// 0b0011 = 0 (white) | 3 (bishop)
			WhiteRook = 4,		// 0b0100 = 0 (white) | 4 (rook)
			WhiteQueen = 5,		// 0b0101 = 0 (white) | 5 (queen)
			WhiteKing = 6,		// 0b0110 = 0 (white) | 6 (king)
			// black pieces (color = 8, so bit 3 = 1)
			BlackPawn = 9,		// 0b1001 = 8 (black) | 1 (pawn)
			BlackKnight = 10,	// 0b1010 = 8 (black) | 2 (knight)
			BlackBishop = 11,	// 0b1011 = 8 (black) | 3 (bishop)
			BlackRook = 12,		// 0b1100 = 8 (black) | 4 (rook)
			BlackQueen = 13,	// 0b1101 = 8 (black) | 5 (queen)
			BlackKing = 14,		// 0b1110 = 8 (black) | 6 (king)
						  // clang-format on
		};
		/// @brief Bit mask for extracting piece type (bits 0-2).
		static constexpr auto HexSeven = 0x7U;
		/// @brief Bit mask for extracting color (bit 3).
		static constexpr auto HexEight = 0x8U;
		UnderlyingType underlying_type_{UnderlyingType::Empty};

		/// @brief Get the underlying type of ChessPiece represented as bits.
		/// @return The underlying type as bits.
		[[nodiscard]] constexpr auto raw_bits() const noexcept -> std::uint8_t
		{
			return static_cast<std::uint8_t>(underlying_type_);
		}

		/// @brief Get the underlying type of ChessPiece.
		/// @return The underlying type.
		[[nodiscard]] constexpr auto raw() const noexcept -> UnderlyingType
		{
			return underlying_type_;
		}
	};
}
