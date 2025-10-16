#pragma once

#include <bit>
#include <cassert>
#include <cstdint>

namespace runestone
{
	/// @class ChessBoard
	/// @brief ChessBoard is a 64-bit bitboard (unsigned long long) for chessboard manipulation.
	///
	/// A bitboard is a compact 64-bit representation of a chessboard where each bit corresponds
	/// to one square (0–63). Bitboards allow extremely fast move generation and attack computations
	/// through low-level bitwise operations.
	///
	/// This class provides a type-safe, constexpr, and inlineable wrapper around a raw `uint64_t`
	/// while maintaining zero-overhead performance in optimized builds.
	///
	/// Square mapping convention:
	/// Bit 0 = A1, Bit 7 = H1, Bit 56 = A8, Bit 63 = H8 (little-endian rank-file mapping).
	///
	/// For more information, see:
	/// - https://www.chessprogramming.org/Bitboards
	/// - https://www.chessprogramming.org/Square_Mapping_Considerations
	class ChessBoard
	{
	public:
		/// @brief value_type represents the underlying unsigned 64-bit integer type.
		using value_type = std::uint64_t;

		/// @brief Default-construct an empty (all-zero) bitboard.
		constexpr ChessBoard() noexcept = default;

		/// @brief Construct a chess board with a raw 64-bit value.
		/// @param value The 64-bit value representing the bit pattern.
		constexpr explicit ChessBoard(value_type value) noexcept : value_(value)
		{
		}

		/// @brief Get the raw underlying bit pattern.
		/// @return The internal 64-bit unsigned value.
		[[nodiscard]] constexpr auto raw() const noexcept -> value_type
		{
			return value_;
		}

		/// @brief Check whether this bitboard is empty (no bits set).
		/// @return True if no bits are set, false otherwise.
		[[nodiscard]] constexpr auto is_empty() const noexcept -> bool
		{
			return value_ == 0ULL;
		}

		/// @brief Count the number of set bits (population count).
		/// @return Number of 1-bits in the bitboard.
		[[nodiscard]] constexpr auto pop_count() const noexcept -> int
		{
			return std::popcount(value_);
		}

		/// @brief Set the bit corresponding to the given square.
		/// @param square The square index (0–63).
		constexpr auto set_square(int square) noexcept -> void
		{
			value_ |= (1ULL << square);
		}

		/// @brief Clear the bit corresponding to the given square.
		/// @param square The square index (0–63).
		constexpr auto clear_square(int square) noexcept -> void
		{
			value_ &= ~(1ULL << square);
		}

		/// @brief Toggle (invert) the bit at a given square.
		/// @param square The square index (0–63).
		constexpr auto toggle_square(int square) noexcept -> void
		{
			value_ ^= (1ULL << square);
		}

		/// @brief Test whether the bit corresponding to a square is set.
		/// @param square The square index (0–63).
		/// @return True if the bit is set, false otherwise.
		[[nodiscard]] constexpr auto test(int square) const noexcept -> bool
		{
			return ((value_ >> square) & 1ULL) != 0ULL;
		}

		/// @brief Find index of least significant set bit (LSB).
		/// @return Square index [0–63] of the least significant 1-bit.
		/// @pre Bitboard must not be empty (undefined if `value_ == 0`).
		[[nodiscard]] constexpr auto least_significant_bit() const noexcept -> int
		{
			assert(value_ != 0ULL && "ChessBoard::least_significant_bit() called on empty bitboard");
			return std::countr_zero(value_);
		}

		/// @brief Find index of most significant set bit (MSB).
		/// @return Square index [0–63] of the most significant 1-bit.
		/// @pre Bitboard must not be empty (undefined if `value_ == 0`).
		[[nodiscard]] constexpr auto most_significant_bit() const noexcept -> int
		{
			assert(value_ != 0ULL && "ChessBoard::most_significant_bit() called on empty bitboard");
			return 63 - std::countl_zero(value_);
		}

		/// @brief Pop (remove and return) the least significant bit (LSB).
		/// @return Square index [0–63] of the bit that was cleared.
		/// @post Removes the least significant 1-bit from the board.
		constexpr auto pop_lsb() noexcept -> int
		{
			const auto square = least_significant_bit();
			value_ &= (value_ - 1);
			return square;
		}

		/// @brief Return the union of two chessboards (bitwise OR).
		/// @param rhs The right-hand ChessBoard operand to combine with this one.
		/// @return A new ChessBoard representing the bitwise OR (union) of `*this` and `rhs`.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard whitePieces(0x000000000000FFFFULL);
		/// ChessBoard blackPieces(0xFFFF000000000000ULL);
		/// ChessBoard occupied = whitePieces.or_(blackPieces);
		/// // occupied now contains all squares with any piece
		/// @endcode
		[[nodiscard]] constexpr auto or_(const ChessBoard& rhs) const noexcept -> ChessBoard
		{
			return ChessBoard(value_ | rhs.value_);
		}

		/// @brief Return the intersection of two chessboards (bitwise AND).
		/// @param rhs The right-hand ChessBoard operand to combine with this one.
		/// @return A new ChessBoard representing the bitwise AND (intersection) of `*this` and `rhs`.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard whiteRooks(0x0000000000000081ULL);  // A1, H1
		/// ChessBoard occupiedFiles(0x0101010101010101ULL); // A-file
		/// ChessBoard result = whiteRooks.and_(occupiedFiles);
		/// // result now contains only the rook on A1
		/// @endcode
		[[nodiscard]] constexpr auto and_(const ChessBoard& rhs) const noexcept -> ChessBoard
		{
			return ChessBoard(value_ & rhs.value_);
		}

		/// @brief Return the symmetric difference of two chessboards (bitwise XOR).
		/// @param rhs The right-hand ChessBoard operand to combine with this one.
		/// @return A new ChessBoard representing the bitwise XOR (symmetric difference) of `*this` and `rhs`.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard beforeMove(0x0000000000000010ULL); // Piece on E1
		/// ChessBoard afterMove (0x0000000000100000ULL); // Piece on E4
		/// ChessBoard diff = beforeMove.xor_(afterMove);
		/// // diff has bits set for both E1 and E4 — the squares that changed
		/// @endcode
		[[nodiscard]] constexpr auto xor_(const ChessBoard& rhs) const noexcept -> ChessBoard
		{
			return ChessBoard(value_ ^ rhs.value_);
		}

		/// @brief Return a ChessBoard with all bits inverted (bitwise NOT).
		/// @return A new ChessBoard representing the bitwise NOT (complement) of `*this`.
		/// The current ChessBoard remains unchanged.
		///
		/// @par Example
		/// @code
		/// ChessBoard occupied(0x00000000000000FFULL);  // Rank 1 occupied
		/// ChessBoard empty = occupied.not_();          // All squares except rank 1 are set
		/// @endcode
		[[nodiscard]] constexpr auto not_() const noexcept -> ChessBoard
		{
			return ChessBoard(~value_);
		}

		/// @brief Perform in-place union (bitwise OR assignment).
		///
		/// Combines this ChessBoard with another using bitwise OR and stores the result
		/// in `*this`. Each bit is set if it was set in either operand.
		///
		/// @param rhs The right-hand ChessBoard operand to combine with this one.
		/// @return Reference to the updated ChessBoard after the operation.
		constexpr auto or_assign(const ChessBoard& rhs) noexcept -> ChessBoard&
		{
			value_ |= rhs.value_;
			return *this;
		}

		/// @brief Perform in-place intersection (bitwise AND assignment).
		///
		/// Updates this ChessBoard by performing a bitwise AND with another.
		/// Only bits set in both operands remain set.
		///
		/// @param rhs The right-hand ChessBoard operand to combine with this one.
		/// @return Reference to the updated ChessBoard after the operation.
		constexpr auto and_assign(const ChessBoard& rhs) noexcept -> ChessBoard&
		{
			value_ &= rhs.value_;
			return *this;
		}

		/// @brief Perform in-place symmetric difference (bitwise XOR assignment).
		///
		/// Updates this ChessBoard by performing a bitwise XOR with another.
		/// Each bit is toggled if it is set in exactly one operand.
		///
		/// @param rhs The right-hand ChessBoard operand to combine with this one.
		/// @return Reference to the updated ChessBoard after the operation.
		constexpr auto xor_assign(const ChessBoard& rhs) noexcept -> ChessBoard&
		{
			value_ ^= rhs.value_;
			return *this;
		}

		/// @brief Check if two ChessBoards are bitwise equal.
		///
		/// Compares the raw bit pattern of this ChessBoard with another.
		/// Returns true if both represent the exact same set of occupied squares.
		///
		/// @param rhs The ChessBoard to compare against.
		/// @return True if both ChessBoards have identical bit patterns, false otherwise.
		[[nodiscard]] constexpr auto equals(const ChessBoard& rhs) const noexcept -> bool
		{
			return value_ == rhs.value_;
		}

		/// @brief Check if two ChessBoards differ bitwise.
		///
		/// Compares the raw bit pattern of this ChessBoard with another.
		/// Returns true if their bit patterns differ in at least one position.
		///
		/// @param rhs The ChessBoard to compare against.
		/// @return True if the ChessBoards differ, false if they are identical.
		[[nodiscard]] constexpr auto not_equals(const ChessBoard& rhs) const noexcept -> bool
		{
			return value_ != rhs.value_;
		}

	private:
		value_type value_{0ULL};
	};

	/// @brief Common chessboard bitboard masks.
	///
	/// These constants define frequently used bitboard masks for files, ranks,
	/// board edges, diagonals, and color squares. They are used during move
	/// generation, attack table construction, and board evaluation.
	///
	/// For more information see:
	/// - https://www.chessprogramming.org/Bitboards
	/// - https://www.chessprogramming.org/Board_Representation
	namespace bitmask
	{
		// File chessboards
		inline constexpr ChessBoard AFile(0x0101010101010101ULL);
		inline constexpr ChessBoard BFile(AFile.raw() << 1U);
		inline constexpr ChessBoard CFile(AFile.raw() << 2U);
		inline constexpr ChessBoard DFile(AFile.raw() << 3U);
		inline constexpr ChessBoard EFile(AFile.raw() << 4U);
		inline constexpr ChessBoard FFile(AFile.raw() << 5U);
		inline constexpr ChessBoard GFile(AFile.raw() << 6U);
		inline constexpr ChessBoard HFile(AFile.raw() << 7U);

		// Rank chessboards
		inline constexpr ChessBoard Rank1(0xFFULL);
		inline constexpr ChessBoard Rank2(Rank1.raw() << (8 * 1U));
		inline constexpr ChessBoard Rank3(Rank1.raw() << (8 * 2U));
		inline constexpr ChessBoard Rank4(Rank1.raw() << (8 * 3U));
		inline constexpr ChessBoard Rank5(Rank1.raw() << (8 * 4U));
		inline constexpr ChessBoard Rank6(Rank1.raw() << (8 * 5U));
		inline constexpr ChessBoard Rank7(Rank1.raw() << (8 * 6U));
		inline constexpr ChessBoard Rank8(Rank1.raw() << (8 * 7U));

		// Edge chessboards
		inline constexpr ChessBoard NotAFile(~AFile.raw());
		inline constexpr ChessBoard NotHFile(~HFile.raw());
		inline constexpr ChessBoard NotRank1(~Rank1.raw());
		inline constexpr ChessBoard NotRank8(~Rank8.raw());

		// Special chessboards
		inline constexpr ChessBoard Empty(0x0000000000000000ULL);
		inline constexpr ChessBoard Full(0xFFFFFFFFFFFFFFFFULL);
		inline constexpr ChessBoard LightSquares(0x55AA55AA55AA55AAULL);
		inline constexpr ChessBoard DarkSquares(0xAA55AA55AA55AA55ULL);
		inline constexpr ChessBoard DiagonalA1H8(0x8040201008040201ULL);
		inline constexpr ChessBoard AntiDiagonalH1A8(0x0102040810204080ULL);
	}
}
